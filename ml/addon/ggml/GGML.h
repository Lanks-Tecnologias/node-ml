//
// Created by denis.montes on 18/04/25.
//

#ifndef GGML_H
#define GGML_H
#include <napi.h>

#include "Device.h"

class GGML: public Napi::ObjectWrap<GGML>
{
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);
    GGML(const Napi::CallbackInfo& info);
    ~GGML();
private:
    static Napi::Value GetDeviceCount(const Napi::CallbackInfo& info);
    static Napi::Value GetDevice(const Napi::CallbackInfo& info);
    static Napi::Value InitApi(const Napi::CallbackInfo& info);
    static Napi::Value FreeApi(const Napi::CallbackInfo& info);
    static Napi::Value CreateGraph(const Napi::CallbackInfo& info);
    static Napi::Value LoadAllBackends(const Napi::CallbackInfo& info);
};
#endif //GGML_H
