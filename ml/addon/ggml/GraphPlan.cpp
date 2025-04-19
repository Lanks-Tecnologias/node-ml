//
// Created by denis.montes on 19/04/25.
//

#include "GraphPlan.h"
Napi::FunctionReference GraphPlan::constructor;
GraphPlan::~GraphPlan()
{
    ggml_backend_graph_plan_free(backend, plan);
}

GraphPlan::GraphPlan(const Napi::CallbackInfo& info):
Napi::ObjectWrap<GraphPlan>(info)
{

}

void GraphPlan::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "GraphPlan", {

    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("GraphPlan", func);
}
