//
// Created by denis.montes on 03/05/25.
//

#ifndef UPSCALER_H
#define UPSCALER_H
#include <napi.h>
#include "stable-diffusion.h"

class Upscaler: public Napi::ObjectWrap<Upscaler> {
public:
  static Napi::FunctionReference constructor;
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  Upscaler(const Napi::CallbackInfo& info);
  ~Upscaler();
  upscaler_ctx_t * context = nullptr;
  Napi::Value Upscale(const Napi::CallbackInfo& info);
};



#endif //UPSCALER_H
