//
// Created by denis.montes on 18/04/25.
//

#include "InitParams.h"

Napi::FunctionReference InitParams::constructor;

void InitParams::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "InitParams", {
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("InitParams", func);
}

InitParams::InitParams(const Napi::CallbackInfo& info)
: Napi::ObjectWrap<InitParams>(info)
{
    Napi::Env env = info.Env();

    if (!info[0].IsObject())
    {
        Napi::TypeError::New(env, "Expected an object").ThrowAsJavaScriptException();
        return;
    }

    auto options = info[0].As<Napi::Object>();

    size_t mem_size = options.Has("memSize")
                          ? options.Get("memSize").As<Napi::Number>().Uint32Value()
                          : 16 * 1024 * 1024;
    bool no_alloc = options.Has("noAlloc") ? options.Get("noAlloc").As<Napi::Boolean>().Value() : false;

    void* mem_buffer = malloc(mem_size);

    this->params.mem_size = mem_size;
    this->params.mem_buffer = mem_buffer;
    this->params.no_alloc = no_alloc;
}

InitParams::~InitParams()
{
    if (params.mem_buffer)
    {
        free(params.mem_buffer);
    }
}
