//
// Created by denis.montes on 18/04/25.
//

#ifndef GGML_H
#define GGML_H
#include <napi.h>

#include "GGMLBackendDevice.h"

class GGML: public Napi::ObjectWrap<GGML>
{
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);
    GGML(const Napi::CallbackInfo& info);
    ~GGML();
private:
    //Napi::Value GetDeviceCount(const Napi::CallbackInfo& info);
};
#endif //GGML_H
