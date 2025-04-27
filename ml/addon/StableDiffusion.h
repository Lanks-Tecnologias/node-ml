//
// Created by denis.montes on 26/04/25.
//

#ifndef STABLEDIFFUSION_H
#define STABLEDIFFUSION_H
#include <napi.h>


class StableDiffusion: public Napi::ObjectWrap<StableDiffusion> {
public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    StableDiffusion(const Napi::CallbackInfo& info);
    ~StableDiffusion();
};



#endif //STABLEDIFFUSION_H
