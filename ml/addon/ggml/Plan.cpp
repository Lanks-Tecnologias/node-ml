//
// Created by denis.montes on 23/04/25.
//

#include "Plan.h"

Napi::FunctionReference Plan::constructor;
void Plan::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Plan", {

    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Plan", func);
}

Plan::~Plan()
{

}


