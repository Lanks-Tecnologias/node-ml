#include <napi.h>

#include "CGraph.h"
#include "ggml-backend.h"
//
// Created by denis.montes on 17/04/25.
//
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
};

Napi::FunctionReference Backend::constructor;

void Backend::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "GgmlBackend", {
            InstanceAccessor("guid", &Backend::GetGuid, nullptr),
            InstanceAccessor("name", &Backend::GetName, nullptr),
            InstanceAccessor("defaultBufferType", &Backend::GetDefaultBufferType, nullptr),
            InstanceAccessor("allocBuffer", &Backend::GetAllocBuffer, nullptr),
            InstanceAccessor("alignment", &Backend::GetAlignment, nullptr),
            InstanceAccessor("maxSize", &Backend::GetMaxSize, nullptr),
            InstanceMethod("setTensorAsync", &Backend::SetTensorAsync),
            InstanceMethod("getTensorAsync", &Backend::GetTensorAsync),
            InstanceMethod("setTensor", &Backend::SetTensor),
            InstanceMethod("getTensor", &Backend::GetTensor),
            InstanceMethod("memSet", &Backend::Memset),
            InstanceMethod("synchronize", &Backend::Synchronize),
            InstanceMethod("createGraphPlan", &Backend::CreateGraphPlan),
            InstanceMethod("freeGraphPlan", &Backend::FreeGraphPlan),
            InstanceMethod("computePlan", &Backend::ComputePlan),
            InstanceMethod("computeGraph", &Backend::ComputeGraph),
            InstanceMethod("computeGraphAsync", &Backend::ComputeGraphAsync),
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("GgmlBackend", func);
}

Backend::~Backend()
{
    ggml_backend_free(backend);
}

Backend::Backend(const Napi::CallbackInfo& info): ObjectWrap<Backend>(info)
{
}

Napi::Value Backend::GetGuid(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), "ggml-backend-dummy");
}

Napi::Value Backend::GetName(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), ggml_backend_name(backend));
}

Napi::Value Backend::GetDefaultBufferType(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value Backend::GetAllocBuffer(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

void Backend::SetTensorAsync(const Napi::CallbackInfo& info)
{

}

void Backend::GetTensorAsync(const Napi::CallbackInfo& info)
{

}

void Backend::SetTensor(const Napi::CallbackInfo& info)
{

}

void Backend::GetTensor(const Napi::CallbackInfo& info)
{

}

Napi::Value Backend::GetAlignment(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), static_cast<double>(ggml_backend_get_alignment(backend)));
}

Napi::Value Backend::GetMaxSize(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), static_cast<double>(ggml_backend_get_max_size(backend)));
}

void Backend::Memset(const Napi::CallbackInfo& info)
{

}

void Backend::Synchronize(const Napi::CallbackInfo& info)
{
    ggml_backend_synchronize(backend);
}

Napi::Value Backend::CreateGraphPlan(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

void Backend::FreeGraphPlan(const Napi::CallbackInfo& info)
{
    ggml_backend_graph_plan_free(backend, nullptr);
}

Napi::Value Backend::ComputePlan(const Napi::CallbackInfo& info)
{
return Napi::Object::New(info.Env());
}

Napi::Value Backend::ComputeGraph(const Napi::CallbackInfo& info)
{
    const auto cgraphObj = info[0].As<Napi::Object>();
    const auto cgraph = Napi::ObjectWrap<CGraph>::Unwrap(cgraphObj);
    const auto status = ggml_backend_graph_compute(backend, cgraph->graph);
    return Napi::Number::New(info.Env(), status);
}

Napi::Value Backend::ComputeGraphAsync(const Napi::CallbackInfo& info)
{
    const auto cgraphObj = info[0].As<Napi::Object>();
    const auto cgraph = Napi::ObjectWrap<CGraph>::Unwrap(cgraphObj);
    const auto status = ggml_backend_graph_compute_async(backend, cgraph->graph);
    return Napi::Number::New(info.Env(), status);
}