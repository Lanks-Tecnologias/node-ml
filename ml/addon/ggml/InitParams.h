//
// Created by denis.montes on 18/04/25.
//

#ifndef INITPARAMS_H
#define INITPARAMS_H
#include <napi.h>

#include "ggml.h"


class InitParams: public Napi::ObjectWrap<InitParams> {
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);
    ggml_init_params params;
    InitParams(const Napi::CallbackInfo& info);
    ~InitParams();
};



#endif //INITPARAMS_H
