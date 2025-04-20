//
// Created by denis.montes on 19/04/25.
//

#ifndef GRAPHPLAN_H
#define GRAPHPLAN_H
#include <napi.h>

#include "ggml-backend.h"


class GraphPlan: public Napi::ObjectWrap<GraphPlan> {
    public:
        static Napi::FunctionReference constructor;
        static void Init(Napi::Env env, Napi::Object exports);
        GraphPlan(const Napi::CallbackInfo& info);
        ~GraphPlan();
        ggml_backend_graph_plan_t plan;
        ggml_backend_t backend;

};



#endif //GRAPHPLAN_H
