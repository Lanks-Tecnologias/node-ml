//
// Created by denis.montes on 22/04/25.
//

#include "BufferType.h"
#include "Buffer.h"
#include "Device.h"
#include "Tensor.h"

Napi::FunctionReference BufferType::constructor;

void BufferType::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "BufferType", {
        InstanceAccessor("name", &BufferType::GetName, nullptr),
        InstanceAccessor("maxSize", &BufferType::GetMaxSize, nullptr),
        InstanceAccessor("alignment", &BufferType::GetAlignment, nullptr),
        InstanceAccessor("isHost", &BufferType::IsHost, nullptr),
        InstanceMethod("allocBuffer", &BufferType::AllocBuffer),
        InstanceMethod("getAllocSize", &BufferType::GetAllocSize),
        InstanceAccessor("device", &BufferType::GetDevice, nullptr),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("BufferType", func);
}

BufferType* BufferType::From(const Napi::CallbackInfo& info, ggml_backend_buffer_type_t buffer_type)
{
    const auto obj = Napi::Object::New(info.Env());
    const auto wrap = Napi::ObjectWrap<BufferType>::Unwrap(obj);
    wrap->bufferType = buffer_type;
    return wrap;
}

BufferType::~BufferType()
{

}

BufferType::BufferType(const Napi::CallbackInfo& info): Napi::ObjectWrap<BufferType>(info)
{

}

Napi::Value BufferType::GetName(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), ggml_backend_buft_name(bufferType));
}

Napi::Value BufferType::GetAllocSize(const Napi::CallbackInfo& info)
{
    const auto tensorObj = info[0].As<Napi::Object>();
    const auto tensorWrap = Napi::ObjectWrap<Tensor>::Unwrap(tensorObj);
    return Napi::Number::New(info.Env(), static_cast<double>(ggml_backend_buft_get_alloc_size(bufferType, tensorWrap->tensor)));
}

Napi::Value BufferType::GetMaxSize(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), static_cast<double>(ggml_backend_buft_get_max_size(bufferType)));
}

Napi::Value BufferType::GetAlignment(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), static_cast<double>(ggml_backend_buft_get_alignment(bufferType)));
}

Napi::Value BufferType::IsHost(const Napi::CallbackInfo& info)
{
    return Napi::Boolean::New(info.Env(), ggml_backend_buft_is_host(bufferType));
}

Napi::Value BufferType::GetDevice(const Napi::CallbackInfo& info)
{
    const auto device = ggml_backend_buft_get_device(bufferType);
    const auto deviceObj = Napi::Object::New(info.Env());
    const auto deviceWrap = Napi::ObjectWrap<Device>::Unwrap(deviceObj);
    deviceWrap->dev = device;
    return deviceObj;
}

Napi::Value BufferType::AllocBuffer(const Napi::CallbackInfo& info)
{
    const auto sizeNumber = info[0].As<Napi::Number>();
    const auto size = sizeNumber.Uint32Value();
    const auto buffer = ggml_backend_buft_alloc_buffer(bufferType, size);
    const auto wrapper = Buffer::From(info, buffer);
    return wrapper->Value();
}



