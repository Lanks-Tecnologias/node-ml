//
// Created by denis.montes on 26/04/25.
//

#ifndef BARK_H
#define BARK_H
#include <napi.h>


class Bark: public Napi::ObjectWrap<Bark> {
public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Bark(const Napi::CallbackInfo& info);
    ~Bark();
    struct bark_context* context = nullptr;
    int32_t seed;
    int n_threads;
    Napi::Value GenerateAudio(const Napi::CallbackInfo& info);
    Napi::Value SaveAudio(const Napi::CallbackInfo& info);
};



#endif //BARK_H
