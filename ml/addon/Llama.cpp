//
// Created by denis.montes on 26/04/25.
//

#include "Llama.h"
#include "llama.h"

Napi::FunctionReference Llama::constructor;
Napi::Object Llama::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Llama", {

    });
    constructor.SuppressDestruct();
    exports.Set("Llama", func);
    return exports;
}

Llama::Llama(const Napi::CallbackInfo& info): Napi::ObjectWrap<Llama>(info)
{
    //const auto model_params = llama_model_default_params();
}

Llama::~Llama()
{

}
