#include "Backend.h"

#include "BackendDevice.h"
#include "CGraph.h"


//

// Created by denis.montes on 17/04/25.

//



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
            InstanceAccessor("device", &Backend::GetDevice, nullptr),
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
    const auto cgraphObj = info[0].As<Napi::Object>(); // [ cgraph
    const auto cgraph = Napi::ObjectWrap<CGraph>::Unwrap(cgraphObj);
    ggml_backend_graph_plan_free(backend, cgraph->graph);
}

Napi::Value Backend::ComputePlan(const Napi::CallbackInfo& info)
{
    const auto cgraphObj = info[0].As<Napi::Object>(); // [ cgraph
    const auto cgraph = Napi::ObjectWrap<CGraph>::Unwrap(cgraphObj);

    const auto status = ggml_backend_graph_plan_compute(backend, cgraph->graph);
    return Napi::Number::New(info.Env(), status);
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

Napi::Value Backend::GetDevice(const Napi::CallbackInfo& info)
{
    const auto dev_t = ggml_backend_get_device(backend);
    const auto devObj = BackendDevice::constructor.New({});
    const auto devWrapper = Napi::ObjectWrap<BackendDevice>::Unwrap(devObj);
    devWrapper->backend = backend;
    devWrapper->dev = dev_t;

    return devObj;
}