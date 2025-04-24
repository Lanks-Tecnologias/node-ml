//
// Created by denis.montes on 22/04/25.
//

#include "Scheduler.h"

#include "Backend.h"
#include "CGraph.h"
#include "Tensor.h"
Napi::FunctionReference Scheduler::constructor;

void Scheduler::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Scheduler", {
        InstanceAccessor("nBackends", &Scheduler::GetNumBackends, nullptr),
        InstanceAccessor("nSplits", &Scheduler::GetNumSplits, nullptr),
        InstanceAccessor("nCopies", &Scheduler::GetNumCopies, nullptr),
        InstanceMethod("reserve", &Scheduler::Reserve),
        InstanceMethod("getBackend", &Scheduler::GetBackend),
        InstanceMethod("getBufferSize", &Scheduler::GetBufferSize),
        InstanceMethod("setTensorBackend", &Scheduler::SetTensorBackend),
        InstanceMethod("getTensorBackend", &Scheduler::GetTensorBackend),
        InstanceMethod("reset", &Scheduler::ResetScheduler),
        InstanceMethod("synchronize", &Scheduler::Synchronize),
        InstanceMethod("allocateGraph", &Scheduler::AllocateGraph),
        InstanceMethod("compute", &Scheduler::Compute),
        InstanceMethod("computeAsync", &Scheduler::ComputeAsync),
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Scheduler", func);
}

Scheduler::~Scheduler()
{
    ggml_backend_sched_free(scheduler);
}

Napi::Value Scheduler::Reserve(const Napi::CallbackInfo& info)
{
    const auto graphObj = info[0].As<Napi::Object>();
    const auto wrapper = Napi::ObjectWrap<CGraph>::Unwrap(graphObj);
    return Napi::Boolean::New(info.Env(), ggml_backend_sched_reserve(scheduler, wrapper->graph));
}

Napi::Value Scheduler::GetNumBackends(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), ggml_backend_sched_get_n_backends(scheduler));
}

Napi::Value Scheduler::GetBackend(const Napi::CallbackInfo& info)
{
    const auto index = info[0].As<Napi::Number>().Uint32Value();
    const auto wrapper = Backend::From(info, ggml_backend_sched_get_backend(scheduler, static_cast<int>(index)));
    return wrapper->Value();
}

Napi::Value Scheduler::GetNumSplits(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), ggml_backend_sched_get_n_splits(scheduler));
}

Napi::Value Scheduler::GetNumCopies(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), ggml_backend_sched_get_n_copies(scheduler));
}

Napi::Value Scheduler::GetBufferSize(const Napi::CallbackInfo& info)
{
    const auto backendObj = info[0].As<Napi::Object>();
    const auto wrapper = Napi::ObjectWrap<Backend>::Unwrap(backendObj);
    const auto size = ggml_backend_sched_get_buffer_size(scheduler, wrapper->backend);
    return Napi::Number::New(info.Env(), static_cast<double>(size));
}

Napi::Value Scheduler::GetTensorBackend(const Napi::CallbackInfo& info)
{
    const auto tensorObj = info[0].As<Napi::Object>();
    const auto wrapper = Napi::ObjectWrap<Tensor>::Unwrap(tensorObj);
    const auto backend = ggml_backend_sched_get_tensor_backend(scheduler, wrapper->tensor);
    const auto backendObj = Napi::Object::New(info.Env());
    const auto backendWrapper = Napi::ObjectWrap<Backend>::Unwrap(backendObj);
    backendWrapper->backend = backend;
    return backendObj;
}

Napi::Value Scheduler::AllocateGraph(const Napi::CallbackInfo& info)
{
    const auto graphObj = info[0].As<Napi::Object>();
    const auto wrapper = Napi::ObjectWrap<CGraph>::Unwrap(graphObj);
    return Napi::Boolean::New(info.Env(), ggml_backend_sched_alloc_graph(scheduler, wrapper->graph));
}

Napi::Value Scheduler::Compute(const Napi::CallbackInfo& info)
{
    const auto graphObj = info[0].As<Napi::Object>();
    const auto wrapper = Napi::ObjectWrap<CGraph>::Unwrap(graphObj);
    const auto status = ggml_backend_sched_graph_compute(scheduler, wrapper->graph);
    return Napi::Number::New(info.Env(), static_cast<double>(status));
}

Napi::Value Scheduler::ComputeAsync(const Napi::CallbackInfo& info)
{
    const auto graphObj = info[0].As<Napi::Object>();
    const auto wrapper = Napi::ObjectWrap<CGraph>::Unwrap(graphObj);
    const auto status = ggml_backend_sched_graph_compute_async(scheduler, wrapper->graph);
    return Napi::Number::New(info.Env(), static_cast<double>(status));
}

void Scheduler::SetTensorBackend(const Napi::CallbackInfo& info)
{
    const auto tensorObj = info[0].As<Napi::Object>();
    const auto wrapper = Napi::ObjectWrap<Tensor>::Unwrap(tensorObj);

    const auto backendObj = info[1].As<Napi::Object>();
    const auto backendWrapper = Napi::ObjectWrap<Backend>::Unwrap(backendObj);

    ggml_backend_sched_set_tensor_backend(scheduler, wrapper->tensor, backendWrapper->backend);
}

void Scheduler::ResetScheduler(const Napi::CallbackInfo& info)
{
    ggml_backend_sched_reset(scheduler);
}

void Scheduler::Synchronize(const Napi::CallbackInfo& info)
{
    ggml_backend_sched_synchronize(scheduler);
}

Scheduler::Scheduler(const Napi::CallbackInfo& info):
Napi::ObjectWrap<Scheduler>(info)
{
    const auto options = info[0].As<Napi::Object>();
    const auto backends = options.Get("backends").As<Napi::Array>();
    const auto backendCount = backends.Length();
    const auto graphSize = options.Get("graphSize").As<Napi::Number>().Uint32Value();
    const auto parallel = options.Get("parallel").As<Napi::Boolean>().Value();
    std::vector<ggml_backend_t> backends_vec{};
    std::vector<ggml_backend_buffer_type_t> bufts_vec{};

    for (size_t i = 0; i < backendCount; ++i)
    {
        const auto backEndObj = backends.Get(i).As<Napi::Object>();
        const auto wrapper = Napi::ObjectWrap<Backend>::Unwrap(backEndObj);
        backends_vec.push_back(wrapper->backend);
        bufts_vec.push_back(ggml_backend_get_default_buffer_type(wrapper->backend));
    }
    scheduler = ggml_backend_sched_new(backends_vec.data(), bufts_vec.data(),static_cast<int>(backendCount), graphSize, parallel);
}

