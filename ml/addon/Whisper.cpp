//
// Created by denis.montes on 26/04/25.
//

#include "Whisper.h"
Napi::FunctionReference Whisper::constructor;

Napi::Object Whisper::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Whisper", {
        InstanceMethod("generate", &Whisper::Generate)
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Whisper", func);
    return exports;
}
Whisper::Whisper(const Napi::CallbackInfo& info): Napi::ObjectWrap<Whisper>(info)
{
    const auto options = info[0].As<Napi::Object>();
    if (!options.Has("modelPath"))
    {
        Napi::TypeError::New(info.Env(), "Missing modelPath").ThrowAsJavaScriptException();
        return;
    }
    const auto modelPath = options.Get("modelPath").As<Napi::String>().Utf8Value();

    struct whisper_context_params cparams = whisper_context_default_params();
    cparams.gpu_device = options.Has("device") ? options.Get("device").As<Napi::Number>() : 0;
    cparams.use_gpu = options.Has("useGpu") ? options.Get("useGpu").As<Napi::Boolean>() : true;
    cparams.flash_attn = true;
    cparams.dtw_aheads_preset = WHISPER_AHEADS_TINY;

    context = whisper_init_from_file_with_params(modelPath.c_str(), cparams);

    if (context == nullptr)
    {
        Napi::TypeError::New(info.Env(), "Unable to load model").ThrowAsJavaScriptException();
        return;
    }
}

Whisper::~Whisper()
{
    whisper_free(context);
}

Napi::Value Whisper::Generate(const Napi::CallbackInfo& info)
{
    const auto fileInput = info[0].As<Napi::String>().Utf8Value();
    const auto language = info[1].As<Napi::String>().Utf8Value();

    //@TODO load wave file
    drwav wav;
    drwav_init_file(&wav, fileInput.c_str(), nullptr);

    if (wav.sampleRate == 0)
    {
        Napi::TypeError::New(info.Env(), "Failed to load wave file").ThrowAsJavaScriptException();
        return info.Env().Null();

    }

    std::vector<float> pcmf32;
    pcmf32.resize(wav.totalPCMFrameCount * wav.channels);
    drwav_read_pcm_frames_f32(&wav, wav.totalPCMFrameCount, pcmf32.data());
    drwav_uninit(&wav);

    whisper_full_params wparams = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);

    if (whisper_full_parallel(context, wparams, pcmf32.data(), pcmf32.size(), 4) != 0) {
        Napi::TypeError::New(info.Env(), "Failed to generate speech").ThrowAsJavaScriptException();
        return info.Env().Null();
    }

    const int n_segments = whisper_full_n_segments(context);
    auto output = std::string();
    for (int i = 0; i < n_segments; ++i)
    {
        output += whisper_full_get_segment_text(context, i);
    }
    return Napi::String::New(info.Env(), output.c_str());
}
