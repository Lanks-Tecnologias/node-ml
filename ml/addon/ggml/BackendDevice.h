//
// Created by denis.montes on 17/04/25.
//

#ifndef BACKENDDEVICE_H
#define BACKENDDEVICE_H
#include <napi.h>
#include "ggml.h"
#include "ggml-backend.h"


class BackendDevice: public Napi::ObjectWrap<BackendDevice>
{
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);
    BackendDevice(const Napi::CallbackInfo& info);
    ~BackendDevice();
    ggml_backend_t backend = nullptr;
    ggml_backend_buffer_type_t buffer_type = nullptr;
    ggml_backend_dev_t dev = nullptr;
    struct ggml_backend_dev_props props{};
private:
    Napi::Value GetType(const Napi::CallbackInfo& info);
    Napi::Value GetName(const Napi::CallbackInfo& info);
    Napi::Value GetDescription(const Napi::CallbackInfo& info);
    BackendDevice*  GetThisInstance(const Napi::CallbackInfo& info);
    Napi::Value GetMemory(const Napi::CallbackInfo& info);
    Napi::Value GetDeviceType(const Napi::CallbackInfo& info);
    Napi::Value GetProps(const Napi::CallbackInfo& info);
    Napi::Value GetBackendReg(const Napi::CallbackInfo& info);
    Napi::Value InitDevice(const Napi::CallbackInfo& info);
    Napi::Value GetBufferType(const Napi::CallbackInfo& info);
    Napi::Value GetHostBufferType(const Napi::CallbackInfo& info);
    Napi::Value GetBufferFromHostPtr(const Napi::CallbackInfo& info);
    Napi::Value SupportsOp(const Napi::CallbackInfo& info);
    Napi::Value SupportsBufferType(const Napi::CallbackInfo& info);
    Napi::Value OffLoadOp(const Napi::CallbackInfo& info);
    Napi::Value GetEventNew(const Napi::CallbackInfo& info);
};
#endif //BACKENDDEVICE_H
