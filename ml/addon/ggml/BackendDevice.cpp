//
// Created by denis.montes on 17/04/25.
//
#include "BackendDevice.h"

#include <ggml-backend-impl.h>

#include "BackendReg.h"

Napi::FunctionReference BackendDevice::constructor;


void BackendDevice::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "BackendDevice", {
        InstanceAccessor("backendRegister", &BackendDevice::GetBackendReg, nullptr),
      InstanceAccessor("name", &BackendDevice::GetName, nullptr),
      InstanceAccessor("description", &BackendDevice::GetDescription, nullptr),
      InstanceAccessor("memory", &BackendDevice::GetMemory, nullptr),
      InstanceAccessor("deviceType", &BackendDevice::GetDeviceType, nullptr),
      InstanceAccessor("props", &BackendDevice::GetProps, nullptr),
      InstanceAccessor("bufferType", &BackendDevice::GetBufferType, nullptr),
      InstanceAccessor("hostBufferType", &BackendDevice::GetHostBufferType, nullptr),
      InstanceAccessor("buferFromHostPtr", &BackendDevice::GetBufferFromHostPtr, nullptr),
      InstanceAccessor("supportsOp", &BackendDevice::SupportsOp, nullptr),
      InstanceAccessor("supportsBufferType", &BackendDevice::SupportsBufferType, nullptr),
      InstanceMethod("init", &BackendDevice::InitDevice),
      InstanceMethod("offLoadOp", &BackendDevice::OffLoadOp),
      InstanceMethod("getEventNew", &BackendDevice::GetEventNew),
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("BackendDevice", func);
}

BackendDevice::BackendDevice(const Napi::CallbackInfo& info)
  : Napi::ObjectWrap<BackendDevice>(info)
{
    printf("BackendDevice::BackendDevice\n");
}

BackendDevice::~BackendDevice() {
    // Backends geralmente não precisam de free
    printf("BackendDevice::~BackendDevice\n");
}

Napi::Value BackendDevice::GetType(const Napi::CallbackInfo& info) {
    return Napi::String::New(info.Env(), "ggml_backend_dev");
}

Napi::Value BackendDevice::GetName(const Napi::CallbackInfo& info)
{
    const BackendDevice*  dev = GetThisInstance(info);
    return Napi::String::New(info.Env(), ggml_backend_dev_name(dev->dev));
}

Napi::Value BackendDevice::GetDescription(const Napi::CallbackInfo& info)
{
    const BackendDevice*  dev = GetThisInstance(info);
    return Napi::String::New(info.Env(), ggml_backend_dev_description(dev->dev));
}

BackendDevice* BackendDevice::GetThisInstance(const Napi::CallbackInfo& info)
{
    const auto obj = info.This().As<Napi::Object>();
    return Napi::ObjectWrap<BackendDevice>::Unwrap(obj);
}

Napi::Value BackendDevice::GetMemory(const Napi::CallbackInfo& info)
{
    const BackendDevice*  dev = GetThisInstance(info);
    size_t free = 0;
    size_t total = 0;
    dev->dev->iface.get_memory(dev->dev, &free, &total);

    const Napi::Object ret = Napi::Object::New(info.Env());
    ret.Set("free", Napi::Number::New(info.Env(), static_cast<double>(free)));
    ret.Set("total", Napi::Number::New(info.Env(), static_cast<double>(total)));
    return ret;

}

Napi::Value BackendDevice::GetDeviceType(const Napi::CallbackInfo& info)
{
    const BackendDevice*  dev = GetThisInstance(info);
    //ggml_backend_dev_get_props(dev->dev, dev->props);
    return Napi::Object::New(info.Env());
}

Napi::Value BackendDevice::GetProps(const Napi::CallbackInfo& info)
{
    const BackendDevice*  dev = GetThisInstance(info);
    //dev->dev->iface.get_props(dev->dev, dev->props);
    return Napi::Object::New(info.Env());
}

Napi::Value BackendDevice::InitDevice(const Napi::CallbackInfo& info)
{
    // const GGMLBackendDevice*  dev = GetThisInstance(info);
    // ggml_backend_dev_get_props(dev->dev, dev->props);
    return Napi::Object::New(info.Env());
}

Napi::Value BackendDevice::SupportsOp(const Napi::CallbackInfo& info)
{
    const BackendDevice*  dev = GetThisInstance(info);
    auto op = info[0].As<Napi::Object>();
    auto supports = dev->dev->iface.supports_op(dev->dev, nullptr);
    return Napi::Boolean::New(info.Env(), supports);
}

Napi::Value BackendDevice::SupportsBufferType(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value BackendDevice::OffLoadOp(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value BackendDevice::GetEventNew(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value BackendDevice::GetBufferType(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value BackendDevice::GetHostBufferType(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value BackendDevice::GetBufferFromHostPtr(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value BackendDevice::GetBackendReg(const Napi::CallbackInfo& info)
{
    const auto backendRegObj = BackendReg::constructor.New({});
    const auto backendRegWrapper = Napi::ObjectWrap<BackendReg>::Unwrap(backendRegObj);
    backendRegWrapper->backendReg = ggml_backend_dev_backend_reg(dev);
    return backendRegObj;

}