//
// Created by denis.montes on 18/04/25.
//

#include "CGraph.h"

#include "Tensor.h"

Napi::FunctionReference CGraph::constructor;
void CGraph::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "CGraph", {
        InstanceAccessor("size", &CGraph::GetSize, nullptr),
        InstanceAccessor("nNodes", &CGraph::GetNumberNodes, nullptr),
        InstanceAccessor("nLeafs", &CGraph::GetNumberLeafs, nullptr),
        InstanceAccessor("nodes", &CGraph::GetNodes, nullptr),
        InstanceAccessor("grads", &CGraph::GetGrads, nullptr),
        InstanceAccessor("gradAccs", &CGraph::GetAccs, nullptr),
        InstanceAccessor("leafs", &CGraph::GetLeafs, nullptr),
        InstanceAccessor("visitedHashSet", &CGraph::GetVisitedHashSet, nullptr),
        InstanceAccessor("order", &CGraph::GetOrder, nullptr),

        InstanceMethod("buildForwardExpand", &CGraph::BuildForwardExpand),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("CGraph", func);
}

CGraph::CGraph(const Napi::CallbackInfo& info): Napi::ObjectWrap<CGraph>(info)
{
    printf("CGraph::CGraph\n");
}

CGraph::~CGraph()
{
    printf("CGraph::~CGraph\n");
}

Napi::Value CGraph::GetSize(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), this->graph->size);
}

Napi::Value CGraph::GetNumberNodes(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), this->graph->n_nodes);
}

Napi::Value CGraph::GetNumberLeafs(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), this->graph->n_leafs);
}

Napi::Value CGraph::GetNodes(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value CGraph::GetGrads(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value CGraph::GetAccs(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value CGraph::GetLeafs(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value CGraph::GetVisitedHashSet(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value CGraph::GetOrder(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

void CGraph::BuildForwardExpand(const Napi::CallbackInfo& info)
{
    const auto tensorObj = info[0].As<Napi::Object>();
    const auto tensor = Napi::ObjectWrap<Tensor>::Unwrap(tensorObj);

    ggml_build_forward_expand(this->graph, tensor->tensor);
}


