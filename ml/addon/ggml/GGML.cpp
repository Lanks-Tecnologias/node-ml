//
// Created by denis.montes on 17/04/25.
//
#include "GGML.h"




Napi::FunctionReference GGML::constructor;

void GGML::Init(Napi::Env env, Napi::Object exports)
{

    exports.Set("loadAllBackends", Napi::Function::New(env, [](const Napi::CallbackInfo& info)
    {
        ggml_backend_load_all();
    }));

    exports.Set("getDeviceCount", Napi::Function::New(env, [](const Napi::CallbackInfo& info)
    {
        const Napi::Env env = info.Env();
        return Napi::Number::New(env, static_cast<double>(ggml_backend_dev_count()));
    }));

    exports.Set("getDevice", Napi::Function::New(env, [](const Napi::CallbackInfo& info)
    {
        const Napi::Env env = info.Env();
        return Napi::Object::New(info.Env());
    }));
}