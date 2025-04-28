//
// Created by denis.montes on 26/04/25.
//

#ifndef STABLEDIFFUSION_H
#define STABLEDIFFUSION_H
#include <napi.h>

#include "stable-diffusion.h"


class StableDiffusion: public Napi::ObjectWrap<StableDiffusion> {
public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    StableDiffusion(const Napi::CallbackInfo& info);
    ~StableDiffusion();
    sd_ctx_t * context;
    sd_image_t * images;
    int imageCount;
    Napi::Value TextToImage(const Napi::CallbackInfo& info);
    Napi::Value SaveImageToPng(const Napi::CallbackInfo& info);
};



#endif //STABLEDIFFUSION_H
