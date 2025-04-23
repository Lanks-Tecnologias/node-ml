//
// Created by denis.montes on 22/04/25.
//

#include "Buffer.h"

#include "BufferType.h"
#include "ggml-backend.h"
#include "Tensor.h"

Napi::FunctionReference Buffer::constructor;
void Buffer::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Buffer", {
        InstanceAccessor("name", &Buffer::GetName, nullptr),
        InstanceAccessor("base", &Buffer::GetBase, nullptr),
        InstanceAccessor("size", &Buffer::GetSize, nullptr),
        InstanceAccessor("alignment", &Buffer::GetAlignment, nullptr),
        InstanceAccessor("maxSize", &Buffer::GetMaxSize, nullptr),
        InstanceAccessor("isHost", &Buffer::IsHost, nullptr),
        InstanceAccessor("type", &Buffer::GetType, nullptr),
        InstanceAccessor("usage", &Buffer::GetUsage, &Buffer::SetUsage),
        InstanceMethod("initTensor", &Buffer::InitTensor),
        InstanceMethod("getAllocSize", &Buffer::GetAllocSize),
        InstanceMethod("clear", &Buffer::Clear),
        InstanceMethod("reset", &Buffer::ResetBuffer)
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Buffer", func);
}

Buffer* Buffer::From(const Napi::CallbackInfo& info, ggml_backend_buffer_t buffer)
{
    const auto obj = Napi::Object::New(info.Env());
    const auto wrap = Napi::ObjectWrap<Buffer>::Unwrap(obj);
    wrap->buffer = buffer;
    return wrap;
}

Buffer::Buffer(const Napi::CallbackInfo& info): Napi::ObjectWrap<Buffer>(info)
{
    buffer = nullptr;
}

Buffer::~Buffer()
{
    ggml_backend_buffer_free(buffer);
}

Napi::Value Buffer::GetName(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), ggml_backend_buffer_name(buffer));
}

Napi::Value Buffer::GetBase(const Napi::CallbackInfo& info)
{
    const auto ptr = ggml_backend_buffer_get_base(buffer);
    const auto basePtr = reinterpret_cast<uintptr_t>(ptr);
    const auto base = static_cast<double>(basePtr);
    return Napi::Number::New(info.Env(), base);
}

Napi::Value Buffer::GetSize(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), static_cast<double>(ggml_backend_buffer_get_size(buffer)));
}

Napi::Value Buffer::GetAlignment(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), static_cast<double>(ggml_backend_buffer_get_alignment(buffer)));
}

Napi::Value Buffer::GetMaxSize(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), static_cast<double>(ggml_backend_buffer_get_max_size(buffer)));
}

Napi::Value Buffer::IsHost(const Napi::CallbackInfo& info)
{
    return Napi::Boolean::New(info.Env(), ggml_backend_buffer_is_host(buffer));
}

Napi::Value Buffer::GetType(const Napi::CallbackInfo& info)
{
    const auto type = ggml_backend_buffer_get_type(buffer);
    const auto typeObj = Napi::Object::New(info.Env());
    const auto typeWrapper = Napi::ObjectWrap<BufferType>::Unwrap(typeObj);
    typeWrapper->bufferType = type;
    return typeObj;
}

Napi::Value Buffer::GetUsage(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), static_cast<double>(ggml_backend_buffer_get_usage(buffer)));
}

void Buffer::SetUsage(const Napi::CallbackInfo& info, const Napi::Value& value)
{
    const auto usage = value.As<Napi::Number>().Uint32Value();
    ggml_backend_buffer_set_usage(buffer, static_cast<ggml_backend_buffer_usage>(usage));
}

void Buffer::ResetBuffer(const Napi::CallbackInfo& info)
{
    ggml_backend_buffer_reset(buffer);
}

void Buffer::Clear(const Napi::CallbackInfo& info)
{
    const auto value = info[0].As<Napi::Number>().Uint32Value();
    ggml_backend_buffer_clear(buffer,static_cast<uint8_t>(value));
}

Napi::Value Buffer::InitTensor(const Napi::CallbackInfo& info)
{
    const auto tensorObj = info[0].As<Napi::Object>();
    const auto tensorWrap = Napi::ObjectWrap<Tensor>::Unwrap(tensorObj);
    const auto status = ggml_backend_buffer_init_tensor(buffer, tensorWrap->tensor);
    return Napi::Number::New(info.Env(), static_cast<double>(status));
}

Napi::Value Buffer::GetAllocSize(const Napi::CallbackInfo& info)
{
    const auto tensorObj = info[0].As<Napi::Object>();
    const auto tensorWrap = Napi::ObjectWrap<Tensor>::Unwrap(tensorObj);
    return Napi::Number::New(info.Env(), static_cast<double>(ggml_backend_buffer_get_alloc_size(buffer, tensorWrap->tensor)));
}

