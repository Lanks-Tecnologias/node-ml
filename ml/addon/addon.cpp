//
// Created by denis.montes on 16/04/25.
//
#include "napi.h"
#include <node.h>

#include "ggml/Backend.h"
#include "ggml/GGML.h"
#include "ggml/BackendDevice.h"
#include "ggml/CGraph.h"
#include "ggml/Context.h"
#include "ggml/GraphPlan.h"
#include "ggml/InitParams.h"
#include "ggml/Tensor.h"

Napi::Object  Initialize(Napi::Env env, Napi::Object exports) {
    //NODE_SET_METHOD(exports, "hello", Method);
    GGML::Init(env, exports);
    GraphPlan::Init(env, exports);
    BackendDevice::Init(env, exports);
    Backend::Init(env, exports);
    InitParams::Init(env, exports);
    Context::Init(env, exports);
    CGraph::Init(env, exports);
    Tensor::Init(env, exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Initialize)

