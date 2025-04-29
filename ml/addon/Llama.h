//
// Created by denis.montes on 26/04/25.
//

#ifndef ADDON_LLAMA_H
#define ADDON_LLAMA_H
#include <napi.h>
#include "llama.h"

class Llama: public Napi::ObjectWrap<Llama> {
public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Llama(const Napi::CallbackInfo& info);
    ~Llama();
    llama_model * model = nullptr;
    const struct llama_vocab* vocab = nullptr;
    Napi::Value Generate(const Napi::CallbackInfo& info);
};



#endif //ADDON_LLAMA_H
