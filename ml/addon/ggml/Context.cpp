//
// Created by denis.montes on 18/04/25.
//

#include "Context.h"

#include "CGraph.h"
#include "Tensor.h"

Napi::FunctionReference Context::constructor;

void Context::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "Context", {
        InstanceMethod("createNewGraph", &Context::CreateNewGraph),
        InstanceMethod("createNewGraphCustom", &Context::CreateNewGraphCustom),
        InstanceMethod("duplicateGraph", &Context::DuplicateGraph),
        InstanceMethod("createTensor2D", &Context::CreateTensor2D),
        InstanceMethod("mulMat", &Context::MulMat),
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Context", func);
}

Context::Context(const Napi::CallbackInfo& info): Napi::ObjectWrap<Context>(info)
{

}

Context::~Context()
{

}

Napi::Value Context::CreateNewGraph(const Napi::CallbackInfo& info)
{
    const auto cgraph = ggml_new_graph(ctx);
    auto graph = CGraph::constructor.New({});
    auto graphWraper = Napi::ObjectWrap<CGraph>::Unwrap(graph);
    graphWraper->graph = cgraph;
    return graph;
}

Napi::Value Context::CreateNewGraphCustom(const Napi::CallbackInfo& info)
{
    const auto size = static_cast<size_t>(info[0].As<Napi::Number>().Uint32Value());
    const auto grads = info[1].As<Napi::Boolean>().Value();
    const auto cgraph = ggml_new_graph_custom(ctx, size, grads);
    auto graph = CGraph::constructor.New({});
    auto graphWraper = Napi::ObjectWrap<CGraph>::Unwrap(graph);
    graphWraper->graph = cgraph;
    return graph;
}

Napi::Value Context::DuplicateGraph(const Napi::CallbackInfo& info)
{
    const auto cgraphObj = info[0].As<Napi::Object>();
    const auto cgraph = Napi::ObjectWrap<CGraph>::Unwrap(cgraphObj)->graph;
    const auto cgraphCopy = ggml_graph_dup(ctx, cgraph);
    auto graph = CGraph::constructor.New({});
    auto graphWrapper = Napi::ObjectWrap<CGraph>::Unwrap(graph);
    graphWrapper->graph = cgraphCopy;
    return graph;
}
Napi::Value Context::MulMat(const Napi::CallbackInfo& info)
{
    const auto tensorAObj = info[0].As<Napi::Object>();
    const auto tensorBObj = info[0].As<Napi::Object>();

    const auto aWrapper = Napi::ObjectWrap<Tensor>::Unwrap(tensorAObj);
    const auto bWrapper = Napi::ObjectWrap<Tensor>::Unwrap(tensorBObj);
    const auto tensor = ggml_mul_mat(ctx, aWrapper->tensor, bWrapper->tensor);

    const auto tensorObj = Tensor::constructor.New({});
    const auto tensorWrapper = Napi::ObjectWrap<Tensor>::Unwrap(tensorObj);
    tensorWrapper->tensor = tensor;
    return tensorObj;
}

Napi::Value Context::CreateTensor2D(const Napi::CallbackInfo& info)
{
    const auto type = info[0].As<Napi::Number>().Uint32Value();
    const auto cols = info[0].As<Napi::Number>().Int64Value();
    const auto rows = info[0].As<Napi::Number>().Int64Value();

    const auto tensor = ggml_new_tensor_2d(ctx, static_cast<ggml_type>(type), cols, rows);
    const auto tensorObj = Tensor::constructor.New({});
    const auto tensorWrapper = Napi::ObjectWrap<Tensor>::Unwrap(tensorObj);
    tensorWrapper->tensor = tensor;
    return tensorObj;
}