//
// Created by denis.montes on 26/04/25.
//

#ifndef LLAMA_H
#define LLAMA_H
#include <napi.h>


class Llama: public Napi::ObjectWrap<Llama> {
public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Llama(const Napi::CallbackInfo& info);
    ~Llama();
};



#endif //LLAMA_H
