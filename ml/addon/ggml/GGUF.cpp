//
// Created by denis.montes on 24/04/25.
//

#include "GGUF.h"

#include "gguf.h"

Napi::FunctionReference GGUF::constructor;
Napi::FunctionReference GGUFContext::constructor;

void GGUF::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "GGUF", {

    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("GGUF", func);

    Napi::Object ggufTypes = Napi::Object::New(env);
    ggufTypes.Set("GGUF_TYPE_UINT8", Napi::Number::New(env, gguf_type::GGUF_TYPE_UINT8));
    ggufTypes.Set("GGUF_TYPE_INT8", Napi::Number::New(env, gguf_type::GGUF_TYPE_INT8));
    ggufTypes.Set("GGUF_TYPE_UINT16", Napi::Number::New(env, gguf_type::GGUF_TYPE_UINT16));
    ggufTypes.Set("GGUF_TYPE_INT16", Napi::Number::New(env, gguf_type::GGUF_TYPE_INT16));
    ggufTypes.Set("GGUF_TYPE_UINT32", Napi::Number::New(env, gguf_type::GGUF_TYPE_UINT32));
    ggufTypes.Set("GGUF_TYPE_INT32", Napi::Number::New(env, gguf_type::GGUF_TYPE_INT32));
    ggufTypes.Set("GGUF_TYPE_FLOAT32", Napi::Number::New(env, gguf_type::GGUF_TYPE_FLOAT32));
    ggufTypes.Set("GGUF_TYPE_BOOL", Napi::Number::New(env, gguf_type::GGUF_TYPE_BOOL));
    ggufTypes.Set("GGUF_TYPE_STRING", Napi::Number::New(env, gguf_type::GGUF_TYPE_STRING));
    ggufTypes.Set("GGUF_TYPE_ARRAY", Napi::Number::New(env, gguf_type::GGUF_TYPE_ARRAY));
    ggufTypes.Set("GGUF_TYPE_UINT64", Napi::Number::New(env, gguf_type::GGUF_TYPE_UINT64));
    ggufTypes.Set("GGUF_TYPE_INT64", Napi::Number::New(env, gguf_type::GGUF_TYPE_INT64));
    ggufTypes.Set("GGUF_TYPE_FLOAT64", Napi::Number::New(env, gguf_type::GGUF_TYPE_FLOAT64));
    ggufTypes.Set("GGUF_TYPE_COUNT", Napi::Number::New(env, gguf_type::GGUF_TYPE_COUNT));

    exports.Set("ggufTypes", ggufTypes);
}

GGUF::GGUF(const Napi::CallbackInfo& info): Napi::ObjectWrap<GGUF>(info)
{

}

GGUF::~GGUF()
{

}

void GGUFContext::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "GGUFContext", {
        InstanceAccessor("version", &GGUFContext::GetVersion, nullptr),
        InstanceAccessor("alignment", &GGUFContext::GetAlignment, nullptr),
        InstanceAccessor("dataOffset", &GGUFContext::GetDataOffset, nullptr),
        InstanceAccessor("nKV", &GGUFContext::GetNKeyValue, nullptr),
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("GGUFContext", func);
}

GGUFContext::GGUFContext(const Napi::CallbackInfo& info): Napi::ObjectWrap<GGUFContext>(info)
{
    ctx = nullptr;
}

Napi::Value GGUFContext::GetVersion(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), gguf_get_version(ctx));
}

Napi::Value GGUFContext::GetAlignment(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), static_cast<double>(gguf_get_alignment(ctx)));
}

Napi::Value GGUFContext::GetDataOffset(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), static_cast<double>(gguf_get_data_offset(ctx)));
}

Napi::Value GGUFContext::GetNKeyValue(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), static_cast<double>(gguf_get_n_kv(ctx)));
}

