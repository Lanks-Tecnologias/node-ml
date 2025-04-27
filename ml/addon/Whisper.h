//
// Created by denis.montes on 26/04/25.
//

#ifndef WHISPER_H
#define WHISPER_H
#include <napi.h>


class Whisper: public Napi::ObjectWrap<Whisper>{
public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Whisper(const Napi::CallbackInfo& info);
    ~Whisper();
};



#endif //WHISPER_H
