//
// Created by denis.montes on 18/04/25.
//

#include "CGraph.h"

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
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("CGraph", func);
}

CGraph::CGraph(const Napi::CallbackInfo& info): Napi::ObjectWrap<CGraph>(info)
{

}

CGraph::~CGraph()
{

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

