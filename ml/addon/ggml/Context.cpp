//
// Created by denis.montes on 18/04/25.
//

#include "Context.h"

Napi::FunctionReference Context::constructor;

void Context::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "Context", {});
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


