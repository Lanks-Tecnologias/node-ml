//
// Created by denis.montes on 22/04/25.
//

#ifndef BUFFERTYPE_H
#define BUFFERTYPE_H
#include <napi.h>

#include "ggml-backend.h"


class BufferType: public Napi::ObjectWrap<BufferType> {
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);
    static BufferType* From(const Napi::CallbackInfo& info, ggml_backend_buffer_type_t buffer_type);
    BufferType(const Napi::CallbackInfo& info);
    ~BufferType();
    ggml_backend_buffer_type_t bufferType;
    Napi::Value GetName(const Napi::CallbackInfo& info);
    Napi::Value GetMaxSize(const Napi::CallbackInfo& info);
    Napi::Value GetAlignment(const Napi::CallbackInfo& info);
    Napi::Value IsHost(const Napi::CallbackInfo& info);
    Napi::Value AllocBuffer(const Napi::CallbackInfo& info);
    Napi::Value GetAllocSize(const Napi::CallbackInfo& info);
    Napi::Value GetDevice(const Napi::CallbackInfo& info);
};



#endif //BUFFERTYPE_H
