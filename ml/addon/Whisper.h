//
// Created by denis.montes on 26/04/25.
//

#ifndef ADDON_WHISPER_H
#define ADDON_WHISPER_H
#include <napi.h>
#include "whisper.h"
#include "thirdparty/dr_wav.h"


class Whisper: public Napi::ObjectWrap<Whisper>{
public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Whisper(const Napi::CallbackInfo& info);
    ~Whisper();
    struct whisper_context* context = nullptr;
    Napi::Value Generate(const Napi::CallbackInfo& info);
};



#endif //ADDON_WHISPER_H
