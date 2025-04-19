//
// Created by denis.montes on 17/04/25.
//
#include "GGML.h"

#include <ggml-backend-impl.h>

#include "CGraph.h"
#include "Context.h"


Napi::FunctionReference GGML::constructor;

void GGML::Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("loadAllBackends", Napi::Function::New(env, [](const Napi::CallbackInfo& info)
    {
        ggml_backend_load_all();
    }));

    exports.Set("getDeviceCount", Napi::Function::New(env, [](const Napi::CallbackInfo& info)
    {
        const Napi::Env env = info.Env();
        return Napi::Number::New(env, static_cast<double>(ggml_backend_dev_count()));
    }));

    exports.Set("getDevice", Napi::Function::New(env, [](const Napi::CallbackInfo& info)
    {
        const Napi::Env env = info.Env();
        const auto index = info[0].As<Napi::Number>();
        auto device = ggml_backend_dev_get(index.Int32Value());
        Napi::Object obj = BackendDevice::constructor.New({});
        BackendDevice* instance = Napi::ObjectWrap<BackendDevice>::Unwrap(obj);
        instance->dev = device;
        return obj;
    }));

    exports.Set("init", Napi::Function::New(env, [](const Napi::CallbackInfo& info)
    {
        const auto obj = info[0].As<Napi::Object>();
        const auto memSize = obj.Get("memSize").As<Napi::Number>().Int32Value();
        const auto noAlloc = obj.Get("noAlloc").As<Napi::Boolean>();

        struct ggml_init_params params = {
            static_cast<size_t>(memSize),
            malloc(memSize),
            noAlloc.Value()
        };
        auto context = ggml_init(params);

        const auto contextObject = Context::constructor.New({});
        Context* instance = Napi::ObjectWrap<Context>::Unwrap(contextObject);
        instance->ctx = context;
        return contextObject;
    }));

    exports.Set("free", Napi::Function::New(env, [](const Napi::CallbackInfo& info)
    {
        const auto obj = info[0].As<Napi::Object>();
        Context* instance = Napi::ObjectWrap<Context>::Unwrap(obj);
        ggml_free(instance->ctx);
        return obj;
    }));

    exports.Set("createGraph", Napi::Function::New(env, [](const Napi::CallbackInfo& info)
    {
        const auto obj = info[0].As<Napi::Object>();
        Context* instance = Napi::ObjectWrap<Context>::Unwrap(obj);
        auto ctx = instance->ctx;
        auto grafo = ggml_new_graph(ctx);

        auto cgraphObj = CGraph::constructor.New({});
        CGraph* cgraph = Napi::ObjectWrap<CGraph>::Unwrap(cgraphObj);
        cgraph->graph = grafo;
        return cgraphObj;
    }));

    exports.Set("getDeviceCount", Napi::Function::New(env, [](const Napi::CallbackInfo& info)
    {
        const Napi::Env env = info.Env();
        return Napi::Number::New(env, static_cast<double>(ggml_backend_dev_count()));
    }));
}
