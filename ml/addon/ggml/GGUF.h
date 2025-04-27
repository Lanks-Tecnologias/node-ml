//
// Created by denis.montes on 24/04/25.
//

#ifndef GGUF_H
#define GGUF_H
#include <napi.h>

#include "CGraph.h"


class GGUF: public Napi::ObjectWrap<GGUF> {
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);
    GGUF(const Napi::CallbackInfo& info);
    ~GGUF();

};

class GGUFContext: public Napi::ObjectWrap<GGUFContext>
{
    public:
        static Napi::FunctionReference constructor;
        static void Init(Napi::Env env, Napi::Object exports);
        explicit GGUFContext(const Napi::CallbackInfo& info);
        ~GGUFContext();
        struct gguf_context* ctx;
        Napi::Value GetVersion(const Napi::CallbackInfo& info);
        Napi::Value GetAlignment(const Napi::CallbackInfo& info);
        Napi::Value GetDataOffset(const Napi::CallbackInfo& info);
        Napi::Value GetNKeyValue(const Napi::CallbackInfo& info);
        Napi::Value FindKey(const Napi::CallbackInfo& info);
        Napi::Value GetKey(const Napi::CallbackInfo& info);
        Napi::Value GetKeyValueType(const Napi::CallbackInfo& info);
        Napi::Value GetArrayType(const Napi::CallbackInfo& info);
};



#endif //GGUF_H
