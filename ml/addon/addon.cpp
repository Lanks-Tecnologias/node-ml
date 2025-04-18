//
// Created by denis.montes on 16/04/25.
//
#include "napi.h"
#include <node.h>

#include "ggml/GGML.h"
#include "ggml/GGMLBackendDevice.h"

Napi::Object  Initialize(Napi::Env env, Napi::Object exports) {
    //NODE_SET_METHOD(exports, "hello", Method);
    GGML::Init(env, exports);
    GGMLBackendDevice::Init(env, exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Initialize)

