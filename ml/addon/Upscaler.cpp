//
// Created by denis.montes on 03/05/25.
//

#include "Upscaler.h"

#include "StableDiffusion.h"
void upscalerLogCallback(enum sd_log_level_t level, const char* log, void* data)
{
    int tag_color;
    const char* level_str;
    //Upscaler* upscaler = (Upscaler*)data;
    switch (level) {
    case SD_LOG_DEBUG:
        tag_color = 37;
        level_str = "DEBUG";
        break;
    case SD_LOG_INFO:
        tag_color = 34;
        level_str = "INFO";
        break;
    case SD_LOG_WARN:
        tag_color = 35;
        level_str = "WARN";
        break;
    case SD_LOG_ERROR:
        tag_color = 31;
        level_str = "ERROR";
        break;
    default: /* Potential future-proofing */
        tag_color = 33;
        level_str = "?????";
        break;
    }

    printf("\033[%d;1m[%-5s]\033[0m %s", tag_color, level_str, log);

}
Napi::FunctionReference Upscaler::constructor;
Napi::Object Upscaler::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Upscaler", {
        InstanceMethod("upscale", &Upscaler::Upscale)
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Upscaler", func);
    return exports;
}

Upscaler::~Upscaler()
{
    if (context!= nullptr)
        free_upscaler_ctx(context);
}

Napi::Value Upscaler::Upscale(const Napi::CallbackInfo& info)
{
    const auto imgObj = info[0].As<Napi::Object>();
    const auto image = Image::Unwrap(imgObj);
    const auto scale = info[1].As<Napi::Number>();

    sd_image_t image_t = sd_image_t();
    image_t.width = image->width;
    image_t.height = image->height;
    image_t.channel = 3;
    image_t.data = image->buffer.Data();
    const auto output = upscale(context, image_t, scale.Int32Value());

    const auto outputImageObj = Image::constructor.New({});
    const auto outputImageWrap = Image::Unwrap(outputImageObj);
    outputImageWrap->height = output.height;
    outputImageWrap->width = output.width;
    outputImageWrap->channels.resize(output.channel);
    outputImageWrap->buffer = Napi::Buffer<uint8_t>::Copy(info.Env(), output.data, output.height * output.width * output.channel);
    return outputImageObj;
}

Upscaler::Upscaler(const Napi::CallbackInfo& info): ObjectWrap<Upscaler>(info)
{
    const auto modelPath = info[0].As<Napi::String>();
    const auto nthreads = info[1].As<Napi::Number>();
    const auto device = info[2].As<Napi::Number>();
    sd_set_log_callback(upscalerLogCallback, this);
    context = new_upscaler_ctx(modelPath.Utf8Value().c_str(), nthreads.Int32Value(), device.Int32Value());
    if (context == nullptr)
    {
        Napi::TypeError::New(info.Env(), "Unable to load model").ThrowAsJavaScriptException();
        return;
    }
}
