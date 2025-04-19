//
// Created by denis.montes on 18/04/25.
//

#ifndef TENSOR_H
#define TENSOR_H
#include <napi.h>

#include "ggml.h"


class Tensor: public Napi::ObjectWrap<Tensor> {
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);
    Tensor(const Napi::CallbackInfo& info);
    ~Tensor();
    ggml_tensor * tensor = nullptr;
    Napi::Value GetType(const Napi::CallbackInfo& info);
    Napi::Value GetBuffer(const Napi::CallbackInfo& info);
    Napi::Value GetNe(const Napi::CallbackInfo& info);
    Napi::Value GetNb(const Napi::CallbackInfo& info);
    Napi::Value GetOp(const Napi::CallbackInfo& info);
    Napi::Value GetOpParams(const Napi::CallbackInfo& info);
    Napi::Value GetFlags(const Napi::CallbackInfo& info);
    Napi::Value GetSrc(const Napi::CallbackInfo& info);
    Napi::Value GetViewSrc(const Napi::CallbackInfo& info);
    Napi::Value GetViewOffs(const Napi::CallbackInfo& info);
    Napi::Value GetData(const Napi::CallbackInfo& info);
    Napi::Value GetName(const Napi::CallbackInfo& info);
};



#endif //TENSOR_H
