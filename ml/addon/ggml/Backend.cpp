#include <napi.h>

#include "ggml-backend.h"
//
// Created by denis.montes on 17/04/25.
//
class GgmlBackend : public Napi::ObjectWrap<GgmlBackend>
{
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);
    GgmlBackend(const Napi::CallbackInfo& info);
    ~GgmlBackend();
    ggml_backend_t backend = nullptr;
    ggml_backend_buffer_type_t buffer_type = nullptr;
    ggml_backend_dev_t dev = nullptr;
};

Napi::FunctionReference GgmlBackend::constructor;

void GgmlBackend::Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("getDeviceCount", Napi::Function::New(env, [](const Napi::CallbackInfo& info)
    {
        const Napi::Env env = info.Env();
        return Napi::Number::New(env, static_cast<double>(ggml_backend_dev_count()));
    }));
}