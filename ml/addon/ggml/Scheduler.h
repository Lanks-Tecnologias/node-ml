//
// Created by denis.montes on 22/04/25.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <napi.h>

#include "ggml-backend.h"


class Scheduler: public Napi::ObjectWrap<Scheduler>{
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);
    Scheduler(const Napi::CallbackInfo& info);
    ~Scheduler();
    ggml_backend_sched_t scheduler;
    Napi::Value Reserve(const Napi::CallbackInfo& info);
    Napi::Value GetNumBackends(const Napi::CallbackInfo& info);
    Napi::Value GetBackend(const Napi::CallbackInfo& info);
    Napi::Value GetNumSplits(const Napi::CallbackInfo& info);
    Napi::Value GetNumCopies(const Napi::CallbackInfo& info);
    Napi::Value GetBufferSize(const Napi::CallbackInfo& info);
    Napi::Value GetTensorBackend(const Napi::CallbackInfo& info);
    Napi::Value AllocateGraph(const Napi::CallbackInfo& info);
    Napi::Value Compute(const Napi::CallbackInfo& info);
    Napi::Value ComputeAsync(const Napi::CallbackInfo& info);
    void SetTensorBackend(const Napi::CallbackInfo& info);
    void ResetScheduler(const Napi::CallbackInfo& info);
    void Synchronize(const Napi::CallbackInfo& info);

};



#endif //SCHEDULER_H
