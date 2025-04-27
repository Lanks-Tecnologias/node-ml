//
// Created by denis.montes on 26/04/25.
//

#include "Encodec.h"

Napi::FunctionReference Encodec::constructor;

Napi::Object Encodec::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Encodec", {
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Encodec", func);
    return exports;
}

Encodec::Encodec(const Napi::CallbackInfo& info): Napi::ObjectWrap<Encodec>(info)
{
    const auto modelPath = info[0].As<Napi::String>();
    const auto offset = info[1].As<Napi::Number>();
    const auto nGpuLayers = info[1].As<Napi::Number>();
    context = encodec_load_model(modelPath.Utf8Value().c_str(), offset, nGpuLayers, nullptr);

    if (context == nullptr)
    {
        Napi::TypeError::New(info.Env(), "Unable to load model").ThrowAsJavaScriptException();
        return;
    }
}

Encodec::~Encodec()
{
    encodec_free(context);
}
