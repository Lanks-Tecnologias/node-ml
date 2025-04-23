//
// Created by denis.montes on 17/04/25.
//
#include "Device.h"

#include <ggml-backend-impl.h>

#include "Backend.h"
#include "BackendReg.h"

Napi::FunctionReference Device::constructor;


void Device::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "Device", {
      InstanceAccessor("backendRegister", &Device::GetBackendReg, nullptr),
      InstanceAccessor("name", &Device::GetName, nullptr),
      InstanceAccessor("description", &Device::GetDescription, nullptr),
      InstanceAccessor("memory", &Device::GetMemory, nullptr),
      InstanceAccessor("deviceType", &Device::GetDeviceType, nullptr),
      //InstanceAccessor("props", &BackendDevice::GetProps, nullptr),
      //InstanceAccessor("bufferType", &BackendDevice::GetBufferType, nullptr),
      //InstanceAccessor("hostBufferType", &BackendDevice::GetHostBufferType, nullptr),
      //InstanceAccessor("buferFromHostPtr", &BackendDevice::GetBufferFromHostPtr, nullptr),
      //InstanceAccessor("supportsOp", &BackendDevice::SupportsOp, nullptr),
      InstanceAccessor("supportsBufferType", &Device::SupportsBufferType, nullptr),
      InstanceMethod("init", &Device::InitDevice),
      //InstanceMethod("offLoadOp", &BackendDevice::OffLoadOp),
      //InstanceMethod("getEventNew", &BackendDevice::GetEventNew),
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Device", func);
}

Device::Device(const Napi::CallbackInfo& info)
  : Napi::ObjectWrap<Device>(info)
{
    printf("BackendDevice::BackendDevice\n");
}

Device::~Device() {
    // Backends geralmente não precisam de free
    printf("BackendDevice::~BackendDevice\n");
}

Napi::Value Device::GetType(const Napi::CallbackInfo& info) {
    return Napi::String::New(info.Env(), "ggml_backend_dev");
}

Napi::Value Device::GetName(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), ggml_backend_dev_name(dev));
}

Napi::Value Device::GetDescription(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), ggml_backend_dev_description(dev));
}



Napi::Value Device::GetMemory(const Napi::CallbackInfo& info)
{

    size_t free = 0;
    size_t total = 0;
    dev->iface.get_memory(dev, &free, &total);

    const Napi::Object ret = Napi::Object::New(info.Env());
    ret.Set("free", Napi::Number::New(info.Env(), static_cast<double>(free)));
    ret.Set("total", Napi::Number::New(info.Env(), static_cast<double>(total)));
    return ret;

}

Napi::Value Device::GetDeviceType(const Napi::CallbackInfo& info)
{
    ggml_backend_dev_get_props(dev, &props);
    return Napi::Number::New(info.Env(), props.type);
}

Napi::Value Device::GetProps(const Napi::CallbackInfo& info)
{
    dev->iface.get_props(dev, &props);
    return Napi::Object::New(info.Env());
}

Napi::Value Device::InitDevice(const Napi::CallbackInfo& info)
{
    const auto params = info[0];
    if (params.IsNull())
    {
        backend = ggml_backend_dev_init(dev, nullptr);
    }else
    {
        backend = ggml_backend_dev_init(dev, params.As<Napi::String>().Utf8Value().c_str());

    }
    const auto backendObj = Backend::constructor.New({});
    const auto backendWrapper = Napi::ObjectWrap<Backend>::Unwrap(backendObj);
    backendWrapper->backend = backend;
    return backendObj;
}

Napi::Value Device::SupportsOp(const Napi::CallbackInfo& info)
{
    auto op = info[0].As<Napi::Object>();
    auto supports = dev->iface.supports_op(dev, nullptr);
    return Napi::Boolean::New(info.Env(), supports);
}

Napi::Value Device::SupportsBufferType(const Napi::CallbackInfo& info)
{
    return Napi::Boolean::New(info.Env(), false);
}

Napi::Value Device::OffLoadOp(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value Device::GetEventNew(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value Device::GetBufferType(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value Device::GetHostBufferType(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value Device::GetBufferFromHostPtr(const Napi::CallbackInfo& info)
{
    return Napi::Object::New(info.Env());
}

Napi::Value Device::GetBackendReg(const Napi::CallbackInfo& info)
{
    const auto backendRegObj = BackendReg::constructor.New({});
    const auto backendRegWrapper = Napi::ObjectWrap<BackendReg>::Unwrap(backendRegObj);
    backendRegWrapper->backendReg = ggml_backend_dev_backend_reg(dev);
    return backendRegObj;

}