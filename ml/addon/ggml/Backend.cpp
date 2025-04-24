#include "Backend.h"

#include "Buffer.h"
#include "BufferType.h"
#include "Device.h"
#include "CGraph.h"
#include "Plan.h"


//

// Created by denis.montes on 17/04/25.

//



Napi::FunctionReference Backend::constructor;

void Backend::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Backend", {
            InstanceAccessor("guid", &Backend::GetGuid, nullptr),
            InstanceAccessor("name", &Backend::GetName, nullptr),
            InstanceAccessor("defaultBufferType", &Backend::GetDefaultBufferType, nullptr),
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
            InstanceMethod("allocBuffer", &Backend::AllocBuffer),
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Backend", func);
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
    const auto bufferType = ggml_backend_get_default_buffer_type(backend);
    const auto wrapper = BufferType::From(info, bufferType);
    return wrapper->Value();
}

Napi::Value Backend::AllocBuffer(const Napi::CallbackInfo& info)
{
    const auto size = info[0].As<Napi::Number>().Uint32Value();
    const auto buffer = ggml_backend_alloc_buffer(backend, size);
    const auto bufferWrapper = Buffer::From(info, buffer);
    return bufferWrapper->Value();
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
    const auto cgraphObj = info[0].As<Napi::Object>(); // [ cgraph
    const auto cgraph = Napi::ObjectWrap<CGraph>::Unwrap(cgraphObj);
    const auto plan_t = ggml_backend_graph_plan_create(backend, cgraph->graph);
    const auto planObj = Napi::Object::New(info.Env());
    const auto wrapper = Napi::ObjectWrap<Plan>::Unwrap(planObj);
    wrapper->plan = plan_t;
    return planObj;
}

void Backend::FreeGraphPlan(const Napi::CallbackInfo& info)
{
    const auto cgraphObj = info[0].As<Napi::Object>(); // [ cgraph
    const auto plan = Napi::ObjectWrap<Plan>::Unwrap(cgraphObj);
    ggml_backend_graph_plan_free(backend, plan->plan);
}

Napi::Value Backend::ComputePlan(const Napi::CallbackInfo& info)
{
    const auto planObj = info[0].As<Napi::Object>(); // [ cgraph
    const auto plan = Napi::ObjectWrap<Plan>::Unwrap(planObj);

    const auto status = ggml_backend_graph_plan_compute(backend, plan->plan);
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
    const auto devObj = Device::constructor.New({});
    const auto devWrapper = Napi::ObjectWrap<Device>::Unwrap(devObj);
    devWrapper->backend = backend;
    devWrapper->dev = dev_t;

    return devObj;
}

Backend* Backend::From(const Napi::CallbackInfo& info, ggml_backend_t ggml_backend)
{
    const auto obj = Napi::Object::New(info.Env());
    const auto wrapper = Napi::ObjectWrap<Backend>::Unwrap(obj);
    wrapper->backend = ggml_backend;
    return wrapper;
}
