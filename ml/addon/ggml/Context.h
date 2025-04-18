//
// Created by denis.montes on 18/04/25.
//

#ifndef CONTEXT_H
#define CONTEXT_H
#include <napi.h>
#include "ggml.h"

class Context: public Napi::ObjectWrap<Context> {
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);
    Context(const Napi::CallbackInfo& info);
    ~Context();
    ggml_context * ctx = nullptr;
};



#endif //CONTEXT_H
