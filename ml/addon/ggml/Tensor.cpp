//
// Created by denis.montes on 18/04/25.
//

#include "Tensor.h"

Napi::FunctionReference Tensor::constructor;

void Tensor::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "Tensor", {
        InstanceAccessor("type", &Tensor::GetType, nullptr),
        InstanceAccessor("buffer", &Tensor::GetBuffer, nullptr),
        InstanceAccessor("ne", &Tensor::GetNe, nullptr),
        InstanceAccessor("nb", &Tensor::GetNb, nullptr),
        InstanceAccessor("op", &Tensor::GetOp, nullptr),
        InstanceAccessor("opParams", &Tensor::GetOpParams, nullptr),
        InstanceAccessor("flags", &Tensor::GetFlags, nullptr),
        InstanceAccessor("src", &Tensor::GetSrc, nullptr),
        InstanceAccessor("viewSrc", &Tensor::GetViewSrc, nullptr),
        InstanceAccessor("viewOffs", &Tensor::GetViewOffs, nullptr),
        InstanceAccessor("data", &Tensor::GetData, nullptr),
        InstanceAccessor("name", &Tensor::GetName, nullptr),
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Tensor", func);
}

Tensor::Tensor(const Napi::CallbackInfo& info): Napi::ObjectWrap<Tensor>(info)
{
    Napi::Env env = info.Env();
    printf("Tensor::Tensor\n");
}

Tensor::~Tensor()
{
    printf("Tensor::~Tensor\n");
}

Napi::Value Tensor::GetBuffer(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value Tensor::GetData(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value Tensor::GetName(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), this->tensor->name);
}

Napi::Value Tensor::GetType(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), this->tensor->type);
}

Napi::Value Tensor::GetNe(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value Tensor::GetNb(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value Tensor::GetOp(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), this->tensor->op);
}

Napi::Value Tensor::GetOpParams(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value Tensor::GetFlags(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value Tensor::GetSrc(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value Tensor::GetViewSrc(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value Tensor::GetViewOffs(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}