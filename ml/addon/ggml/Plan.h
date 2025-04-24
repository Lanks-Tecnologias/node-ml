//
// Created by denis.montes on 23/04/25.
//

#ifndef PLAN_H
#define PLAN_H
#include <napi.h>

#include "ggml-backend.h"


class Plan: public Napi::ObjectWrap<Plan> {
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);
    Plan(const Napi::CallbackInfo& info);
    ~Plan();
    ggml_backend_graph_plan_t plan;
};



#endif //PLAN_H
