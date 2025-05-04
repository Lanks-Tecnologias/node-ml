//
// Created by denis.montes on 26/04/25.
//

#include "Llama.h"

#include <iostream>

void llama_log_callback(enum ggml_log_level level, const char * text, void * user_data)
{
    int tag_color;
    const char* level_str;
    //Upscaler* upscaler = (Upscaler*)data;
    switch (level) {
    case GGML_LOG_LEVEL_DEBUG:
        tag_color = 37;
        level_str = "DEBUG";
        break;
    case GGML_LOG_LEVEL_INFO:
        tag_color = 34;
        level_str = "INFO";
        break;
    case GGML_LOG_LEVEL_WARN:
        tag_color = 35;
        level_str = "WARN";
        break;
    case GGML_LOG_LEVEL_ERROR:
        tag_color = 31;
        level_str = "ERROR";
        break;
    default: /* Potential future-proofing */
        tag_color = 33;
        level_str = "";
        printf("\033[%d;1m%s\033[0m",tag_color,text);
        return;
    }

    printf("\033[%d;1m[llama][%-5s]\033[0m %s", tag_color, level_str, text);
}
Napi::FunctionReference Llama::constructor;
Napi::Object Llama::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Llama", {
        InstanceMethod("generate", &Llama::Generate)
    });
    constructor.SuppressDestruct();
    exports.Set("Llama", func);

    llama_backend_init();
    return exports;
}

Llama::Llama(const Napi::CallbackInfo& info): Napi::ObjectWrap<Llama>(info)
{
    const auto options = info[0].As<Napi::Object>();
    const auto device = options.Has("mainDevice") ? options.Get("mainDevice").As<Napi::Number>().Int32Value() : 0;
    const auto modelPath = options.Get("modelPath").As<Napi::String>();
    ggml_backend_load_all();
    llama_log_set(llama_log_callback, this);
    llama_model_params model_params = llama_model_default_params();
    model_params.n_gpu_layers = options.Has("nGpuLayers") ? options.Get("nGpuLayers").As<Napi::Number>().Int32Value() : 99;
    model_params.main_gpu = device;
    model_params.split_mode = LLAMA_SPLIT_MODE_LAYER;
    const auto devList = options.Has("devices") ? options.Get("devices").As<Napi::Array>() : Napi::Array::New(info.Env());
    if (devList.Length() > 0)
    {
        auto itemCount = devList.Length() + 1;
        model_params.devices = static_cast<ggml_backend_dev_t*>(malloc(sizeof(ggml_backend_dev_t) * itemCount));
        for (int i = 0; i < devList.Length(); i++)
        {
            model_params.devices[i] = ggml_backend_dev_get(devList.Get(i).As<Napi::Number>().Int32Value());
        }
        model_params.devices[itemCount-1] = nullptr;
    }else
    {
        const auto nDevices = ggml_backend_dev_count();
        model_params.devices = static_cast<ggml_backend_dev_t*>(malloc(sizeof(ggml_backend_dev_t) * nDevices + 1));
        for (int i = 0; i < nDevices; i++)
        {
            model_params.devices[i] = ggml_backend_dev_get(i);
        }
        model_params.devices[nDevices-1] = nullptr;
    }

    //model_params.devices = devices.data();
    model = llama_model_load_from_file(modelPath.Utf8Value().c_str(), model_params);
    this->vocab = llama_model_get_vocab(model);
    if (model == nullptr) {
        fprintf(stderr , "%s: error: unable to load model\n" , __func__);
        return;
    }
}

Llama::~Llama()
{
    llama_model_free(model);
}

Napi::Value Llama::Generate(const Napi::CallbackInfo& info)
{
    const auto prompt = info[0].As<Napi::String>().Utf8Value();
    int n_ctx =  info[1].As<Napi::Number>();
    // initialize the context
    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx = n_ctx;
    ctx_params.n_batch = n_ctx;

    llama_context * ctx = llama_init_from_model(model, ctx_params);
    if (!ctx) {
        Napi::TypeError::New(info.Env(), "error: failed to create the llama_context\n").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    // initialize the sampler
    llama_sampler * smpl = llama_sampler_chain_init(llama_sampler_chain_default_params());
    llama_sampler_chain_add(smpl, llama_sampler_init_min_p(0.05f, 1));
    llama_sampler_chain_add(smpl, llama_sampler_init_temp(0.8f));
    llama_sampler_chain_add(smpl, llama_sampler_init_dist(LLAMA_DEFAULT_SEED));

    // helper function to evaluate a prompt and generate a response
    auto generate = [&](const std::string & prompt) {
        std::string response;

        const bool is_first = llama_kv_self_used_cells(ctx) == 0;

        // tokenize the prompt
        const int n_prompt_tokens = -llama_tokenize(vocab, prompt.c_str(), prompt.size(), NULL, 0, is_first, true);
        std::vector<llama_token> prompt_tokens(n_prompt_tokens);
        if (llama_tokenize(vocab, prompt.c_str(), prompt.size(), prompt_tokens.data(), prompt_tokens.size(), is_first, true) < 0) {
            GGML_ABORT("failed to tokenize the prompt\n");
        }

        // prepare a batch for the prompt
        llama_batch batch = llama_batch_get_one(prompt_tokens.data(), prompt_tokens.size());
        llama_token new_token_id;
        while (true) {
            // check if we have enough space in the context to evaluate this batch
            int n_ctx = llama_n_ctx(ctx);
            int n_ctx_used = llama_kv_self_used_cells(ctx);
            if (n_ctx_used + batch.n_tokens > n_ctx) {
                printf("\033[0m\n");
                fprintf(stderr, "context size exceeded\n");
                exit(0);
            }

            if (llama_decode(ctx, batch)) {
                GGML_ABORT("failed to decode\n");
            }

            // sample the next token
            new_token_id = llama_sampler_sample(smpl, ctx, -1);

            // is it an end of generation?
            if (llama_vocab_is_eog(vocab, new_token_id)) {
                break;
            }

            // convert the token to a string, print it and add it to the response
            char buf[256];
            int n = llama_token_to_piece(vocab, new_token_id, buf, sizeof(buf), 0, true);
            if (n < 0) {
                GGML_ABORT("failed to convert token to piece\n");
            }

            response += std::string(buf, n);

            // prepare the next batch with the sampled token
            batch = llama_batch_get_one(&new_token_id, 1);
        }

        return response;
    };

    std::vector<llama_chat_message> messages;
    std::vector<char> formatted(llama_n_ctx(ctx));
    int prev_len = 0;

    const char * tmpl = llama_model_chat_template(model, /* name */ nullptr);

    // add the user input to the message list and format it
    messages.push_back({"user", strdup(prompt.c_str())});
    int new_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), true, formatted.data(), formatted.size());
    if (new_len > (int)formatted.size()) {
        formatted.resize(new_len);
        new_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), true, formatted.data(), formatted.size());
    }
    if (new_len < 0) {
        Napi::TypeError::New(info.Env(), "failed to apply the chat template\n").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    // remove previous messages to obtain the prompt to generate the response
   // std::string prompt(formatted.begin() + prev_len, formatted.begin() + new_len);

    // generate a response
    printf("\033[33m");
    std::string response = generate(prompt);
    printf("\n\033[0m");

    // add the response to the messages
    messages.push_back({"assistant", strdup(response.c_str())});
    prev_len = llama_chat_apply_template(tmpl, messages.data(), messages.size(), false, nullptr, 0);
    if (prev_len < 0) {
        Napi::TypeError::New(info.Env(), "failed to apply the chat template\n").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    // free resources
    for (auto & msg : messages) {
        free(const_cast<char *>(msg.content));
    }

    llama_sampler_free(smpl);
    llama_free(ctx);
    return Napi::String::New(info.Env(), response.c_str());
}
