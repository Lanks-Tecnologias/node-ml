//
// Created by denis.montes on 17/04/25.
//
#include "GGMLBackendDevice.h"

Napi::FunctionReference GGMLBackendDevice::constructor;


void GGMLBackendDevice::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "GGMLBackendDevice", {
      InstanceMethod("getName", &GGMLBackendDevice::GetName),
      InstanceMethod("getDescription", &GGMLBackendDevice::GetDescription),
      InstanceMethod("getMemory", &GGMLBackendDevice::GetMemory),
      InstanceMethod("getDeviceType", &GGMLBackendDevice::GetDeviceType),
      InstanceMethod("getProps", &GGMLBackendDevice::GetProps),
      InstanceMethod("init", &GGMLBackendDevice::InitDevice),
      InstanceMethod("getBufferType", &GGMLBackendDevice::GetBufferType),
      InstanceMethod("getHostBufferType", &GGMLBackendDevice::GetHostBufferType),
      InstanceMethod("getBufferFromHostPtr", &GGMLBackendDevice::GetBufferFromHostPtr),
      InstanceMethod("supportsOp", &GGMLBackendDevice::SupportsOp),
      InstanceMethod("supportsBufferType", &GGMLBackendDevice::SupportsBufferType),
      InstanceMethod("offLoadOp", &GGMLBackendDevice::OffLoadOp),
      InstanceMethod("getEventNew", &GGMLBackendDevice::GetEventNew),
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("GGMLBackendDevice", func);
}

GGMLBackendDevice::GGMLBackendDevice(const Napi::CallbackInfo& info)
  : Napi::ObjectWrap<GGMLBackendDevice>(info)
{
    Napi::Object obj = GGMLBackendDevice::constructor.New({});
    GGMLBackendDevice* instance = Napi::ObjectWrap<GGMLBackendDevice>::Unwrap(obj);

   // ggml_backend_dev_get_props(instance->dev, instance->props);
}

GGMLBackendDevice::~GGMLBackendDevice() {
    // Backends geralmente não precisam de free
}

Napi::Value GGMLBackendDevice::GetType(const Napi::CallbackInfo& info) {
    return Napi::String::New(info.Env(), "ggml_backend_dev");
}

Napi::Value GGMLBackendDevice::GetName(const Napi::CallbackInfo& info)
{
    const GGMLBackendDevice*  dev = GetThisInstance(info);
    return Napi::String::New(info.Env(), ggml_backend_dev_name(dev->dev));
}

Napi::Value GGMLBackendDevice::GetDescription(const Napi::CallbackInfo& info)
{
    const GGMLBackendDevice*  dev = GetThisInstance(info);
    return Napi::String::New(info.Env(), ggml_backend_dev_description(dev->dev));
}

GGMLBackendDevice* GGMLBackendDevice::GetThisInstance(const Napi::CallbackInfo& info)
{
    const auto obj = info.This().As<Napi::Object>();
    return Napi::ObjectWrap<GGMLBackendDevice>::Unwrap(obj);
}

Napi::Value GGMLBackendDevice::GetMemory(const Napi::CallbackInfo& info)
{
    const GGMLBackendDevice*  dev = GetThisInstance(info);
    ggml_backend_dev_get_props(dev->dev, dev->props);

    const Napi::Object ret = Napi::Object::New(info.Env());
    ret.Set("free", Napi::Number::New(info.Env(), static_cast<double>(dev->props->memory_free)));
    ret.Set("total", Napi::Number::New(info.Env(), static_cast<double>(dev->props->memory_total)));
    return ret;

}

Napi::Value GGMLBackendDevice::GetDeviceType(const Napi::CallbackInfo& info)
{
    const GGMLBackendDevice*  dev = GetThisInstance(info);
    ggml_backend_dev_get_props(dev->dev, dev->props);
    return Napi::Object::New(info.Env());
}

Napi::Value GGMLBackendDevice::GetProps(const Napi::CallbackInfo& info)
{
    const GGMLBackendDevice*  dev = GetThisInstance(info);
    ggml_backend_dev_get_props(dev->dev, dev->props);
    return Napi::Object::New(info.Env());
}

Napi::Value GGMLBackendDevice::InitDevice(const Napi::CallbackInfo& info)
{
    // const GGMLBackendDevice*  dev = GetThisInstance(info);
    // ggml_backend_dev_get_props(dev->dev, dev->props);
    return Napi::Object::New(info.Env());
}

Napi::Value GGMLBackendDevice::SupportsOp(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value GGMLBackendDevice::SupportsBufferType(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value GGMLBackendDevice::OffLoadOp(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value GGMLBackendDevice::GetEventNew(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value GGMLBackendDevice::GetBufferType(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value GGMLBackendDevice::GetHostBufferType(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value GGMLBackendDevice::GetBufferFromHostPtr(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}