//
// Created by denis.montes on 19/04/25.
//

#ifndef BACKEND_H
#define BACKEND_H
#include <napi.h>
#include "ggml-backend.h"
class Backend : public Napi::ObjectWrap<Backend>
{
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);
    Backend(const Napi::CallbackInfo& info);
    ~Backend();
    ggml_backend_t backend = nullptr;
    ggml_backend_buffer_type_t buffer_type = nullptr;
    ggml_backend_dev_t dev = nullptr;
    Napi::Value GetGuid(const Napi::CallbackInfo& info);
    Napi::Value GetName(const Napi::CallbackInfo& info);
    Napi::Value GetDefaultBufferType(const Napi::CallbackInfo& info);
    Napi::Value GetAllocBuffer(const Napi::CallbackInfo& info);
    void SetTensorAsync(const Napi::CallbackInfo& info);
    void GetTensorAsync(const Napi::CallbackInfo& info);
    void SetTensor(const Napi::CallbackInfo& info);
    void GetTensor(const Napi::CallbackInfo& info);
    Napi::Value GetAlignment(const Napi::CallbackInfo& info);
    Napi::Value GetMaxSize(const Napi::CallbackInfo& info);
    void Memset(const Napi::CallbackInfo& info);
    void Synchronize(const Napi::CallbackInfo& info);
    Napi::Value CreateGraphPlan(const Napi::CallbackInfo& info);
    void FreeGraphPlan(const Napi::CallbackInfo& info);
    Napi::Value ComputePlan(const Napi::CallbackInfo& info);
    Napi::Value ComputeGraph(const Napi::CallbackInfo& info);
    Napi::Value ComputeGraphAsync(const Napi::CallbackInfo& info);
    Napi::Value GetDevice(const Napi::CallbackInfo& info);
};
#endif //BACKEND_H
