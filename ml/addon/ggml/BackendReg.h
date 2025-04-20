//
// Created by denis.montes on 19/04/25.
//

#ifndef BACKENDREG_H
#define BACKENDREG_H
#include <napi.h>

#include "ggml-backend.h"


class BackendReg: public Napi::ObjectWrap<BackendReg>{
    public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);
    BackendReg(const Napi::CallbackInfo& info);
    ~BackendReg();
    ggml_backend_reg_t backendReg;
    Napi::Value GetName(const Napi::CallbackInfo& info);
    Napi::Value GetDeviceCount(const Napi::CallbackInfo& info);
    Napi::Value GetDevice(const Napi::CallbackInfo& info);
};



#endif //BACKENDREG_H
