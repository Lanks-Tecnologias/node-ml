//
// Created by denis.montes on 26/04/25.
//

#include "Bark.h"

#include <thread>

#include "bark.h"
Napi::FunctionReference Bark::constructor;
Napi::Object Bark::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env,"Bark", {
        InstanceMethod("generateAudio", &Bark::GenerateAudio),
        InstanceMethod("saveAudio", &Bark::SaveAudio)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Bark", func);


    return exports;
}
Bark::~Bark()
{
    bark_free(context);
}

Napi::Value Bark::GenerateAudio(const Napi::CallbackInfo& info)
{
    const auto prompt = info[0].As<Napi::String>().Utf8Value();
    const auto result = bark_generate_audio(context, prompt.c_str(), n_threads);
    return Napi::Boolean::New(info.Env(), result);
}

Napi::Value Bark::SaveAudio(const Napi::CallbackInfo& info)
{
    const auto output_path = info[0].As<Napi::String>().Utf8Value();
    const float *audio_data = bark_get_audio_data(context);
    if (audio_data == nullptr) {
        Napi::TypeError::New(info.Env(), "Could not get audio data\n").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    const int audio_arr_size = bark_get_audio_data_size(context);
    std::vector<float> audio_arr(audio_data, audio_data + audio_arr_size);

    // @TODO Write to wave file
    return info.Env().Undefined();

}

Bark::Bark(const Napi::CallbackInfo& info): Napi::ObjectWrap<Bark>(info)
{
    const auto obj = info[0].As<Napi::Object>();

    this->n_threads = std::min(4, static_cast<int32_t>(std::thread::hardware_concurrency()));
    if (obj.Has("nThreads"))
    {
        n_threads = obj.Get("nThreads").As<Napi::Number>().Int32Value();
    }

    if (!obj.Has("modelPath"))
    {
        Napi::TypeError::New(info.Env(), "Expected a model path").ThrowAsJavaScriptException();
        return;
    }

    const auto model_path = obj.Get("modelPath").As<Napi::String>().Utf8Value();


    this->seed = obj.Has("seed") ? obj.Get("seed").As<Napi::Number>().Int32Value() : 0;
    ggml_backend_load_all();
    const auto devIndex = obj.Has("device") ? obj.Get("device").As<Napi::Number>().Int32Value() : 0;
    const auto dev = ggml_backend_dev_get(devIndex);
    const auto backend  = ggml_backend_dev_init(dev, nullptr);
    struct bark_context_params ctx_params = bark_context_default_params();
    ctx_params.verbosity = HIGH;
    this->context = bark_load_model(model_path.c_str(),ctx_params, seed, backend);
    if (!context) {
        Napi::TypeError::New(info.Env(), "Could not load model\n").ThrowAsJavaScriptException();
        return;
    }
}
