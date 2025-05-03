//
// Created by denis.montes on 26/04/25.
//

#ifndef ADDON_STABLEDIFFUSION_H
#define ADDON_STABLEDIFFUSION_H
#include <napi.h>
#include "stable-diffusion.h"
struct pixel_channel
{
    uint8_t size;
    uint8_t index;
    std::string name;
};
class Image: public Napi::ObjectWrap<Image>
{
public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Image(const Napi::CallbackInfo& info);
    ~Image();
    Napi::Buffer<uint8_t> buffer;
    u_int32_t width;
    u_int32_t height;
    std::vector<pixel_channel> channels;
    Napi::Value GetWidth(const Napi::CallbackInfo& info);
    Napi::Value GetHeight(const Napi::CallbackInfo& info);
    Napi::Value GetData(const Napi::CallbackInfo& info);
    Napi::Value GetChannels(const Napi::CallbackInfo& info);
    Napi::Value GetPixelSize(const Napi::CallbackInfo& info);
    Napi::Value LoadFromFile(const Napi::CallbackInfo& info);
    Napi::Value SaveFile(const Napi::CallbackInfo& info);
};

class StableDiffusion: public Napi::ObjectWrap<StableDiffusion> {
public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    StableDiffusion(const Napi::CallbackInfo& info);
    ~StableDiffusion();
    sd_ctx_t * context = nullptr;
    sd_image_t * images = nullptr;
    int imageCount;
    Napi::Value TextToImage(const Napi::CallbackInfo& info);
    Napi::Value SaveImageToPng(const Napi::CallbackInfo& info);
    Napi::Value ImageToImage(const Napi::CallbackInfo& info);
};



#endif //ADDON_STABLEDIFFUSION_H
