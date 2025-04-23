//
// Created by denis.montes on 22/04/25.
//

#ifndef BUFFER_H
#define BUFFER_H
#include <napi.h>

#include "ggml-backend.h"


class Buffer: public Napi::ObjectWrap<Buffer>{
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);
    static Buffer* From(const Napi::CallbackInfo& info, ggml_backend_buffer_t buffer);
    Buffer(const Napi::CallbackInfo& info);
    ~Buffer();
    ggml_backend_buffer_t buffer;
    Napi::Value GetName(const Napi::CallbackInfo& info);
    Napi::Value GetBase(const Napi::CallbackInfo& info);
    Napi::Value GetSize(const Napi::CallbackInfo& info);
    Napi::Value GetAlignment(const Napi::CallbackInfo& info);
    Napi::Value GetMaxSize(const Napi::CallbackInfo& info);
    Napi::Value IsHost(const Napi::CallbackInfo& info);
    Napi::Value GetType(const Napi::CallbackInfo& info);
    Napi::Value GetUsage(const Napi::CallbackInfo& info);
    void SetUsage(const Napi::CallbackInfo& info, const Napi::Value& value);
    void ResetBuffer(const Napi::CallbackInfo& info);
    void Clear(const Napi::CallbackInfo& info);
    Napi::Value InitTensor(const Napi::CallbackInfo& info);
    Napi::Value GetAllocSize(const Napi::CallbackInfo& info);
};



#endif //BUFFER_H
