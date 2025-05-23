//
// Created by denis.montes on 19/04/25.
//

#include "BackendReg.h"

#include "Device.h"

Napi::FunctionReference BackendReg::constructor;

void BackendReg::Init(Napi::Env env, Napi::Object exports)
{
    const auto func = DefineClass(env,"BackendReg", {
        InstanceAccessor("name", &BackendReg::GetName, nullptr),
        InstanceAccessor("deviceCount", &BackendReg::GetDeviceCount, nullptr),
        InstanceMethod("getDevice", &BackendReg::GetDevice),
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("BackendReg", func);
}

BackendReg::~BackendReg()
{
    printf("BackendReg::~BackendReg\n");
}

BackendReg::BackendReg(const Napi::CallbackInfo& info): Napi::ObjectWrap<BackendReg>(info)
{
    printf("BackendReg::BackendReg\n");
}

Napi::Value BackendReg::GetName(const Napi::CallbackInfo& info)
{
    printf("BackendReg::GetName\n");
    return Napi::String::New(info.Env(), ggml_backend_reg_name(backendReg));
}

Napi::Value BackendReg::GetDevice(const Napi::CallbackInfo& info)
{
    printf("BackendReg::GetDevice\n");
    const auto index = static_cast<size_t>(info[0].As<Napi::Number>().Uint32Value());
    const auto devObj = Device::constructor.New({});
    const auto devWrap = Napi::ObjectWrap<Device>::Unwrap(devObj);
    devWrap->dev = ggml_backend_reg_dev_get(backendReg, index);
    return devObj;
}

Napi::Value BackendReg::GetDeviceCount(const Napi::CallbackInfo& info)
{
    printf("BackendReg::GetDeviceCount\n");
    return Napi::Number::New(info.Env(), static_cast<double>(ggml_backend_reg_dev_count(backendReg)));
}
