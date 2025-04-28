//
// Created by denis.montes on 26/04/25.
//

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "stb_image_write.h"
#include "StableDiffusion.h"

#include "stable-diffusion.h"

Napi::FunctionReference StableDiffusion::constructor;

Napi::Object StableDiffusion::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "StableDiffusion", {
        InstanceMethod("textToImage", &StableDiffusion::TextToImage),
        InstanceMethod("saveImageToPng", &StableDiffusion::SaveImageToPng)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("StableDiffusion", func);
    return exports;
}

StableDiffusion::StableDiffusion(const Napi::CallbackInfo& info): Napi::ObjectWrap<StableDiffusion>(info)
{
    const auto options = info[0].As<Napi::Object>();
    int device = 0;
    if (options.Has("device"))
    {
        device = options.Get("device").As<Napi::Number>();
    }
    if (!options.Has("modelPath"))
    {
        Napi::TypeError::New(info.Env(), "Missing modelPath").ThrowAsJavaScriptException();
        return;
    }

    const auto modelPath = options.Get("modelPath").As<Napi::String>().Utf8Value();

    if (!options.Has("diffusionModelPath"))
    {
        Napi::TypeError::New(info.Env(), "Missing diffusionModelPath").ThrowAsJavaScriptException();
        return;
    }

    const auto diffusionModelPath = options.Get("modelPath").As<Napi::String>().Utf8Value();
    const std::string clip_l_Path;
    const std::string clip_g_Path;
    const std::string vaePath;
    const std::string taePath;
    const std::string ctrNetPath;
    const std::string loraPath;
    const std::string embedDir;
    context = new_sd_ctx(modelPath.c_str(), clip_l_Path.c_str(), clip_g_Path.c_str(), "",
    diffusionModelPath.c_str(), vaePath.c_str(), taePath.c_str(), ctrNetPath.c_str(),
        loraPath.c_str(), embedDir.c_str(), "", true, true, false, 16, sd_type_t::SD_TYPE_F32,
        rng_type_t::STD_DEFAULT_RNG, DEFAULT, false, true, false, true
    )
    ;
}

StableDiffusion::~StableDiffusion()
{
    free(images);
    free_sd_ctx(context);
}

Napi::Value StableDiffusion::TextToImage(const Napi::CallbackInfo& info)
{
    const auto params = info[0].As<Napi::Object>();
    if (!params.Has("prompt"))
    {
        Napi::TypeError::New(info.Env(), "Missing prompt").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    const auto prompt = params.Get("prompt").As<Napi::String>().Utf8Value();

    if (!params.Has("negativePrompt"))
    {
        Napi::TypeError::New(info.Env(), "Missing prompt").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    const auto negativePrompt = params.Get("negativePrompt").As<Napi::String>().Utf8Value();
    const auto clip_skip = params.Has("clipSkip")? params.Get("clipSkip").As<Napi::Number>().Int32Value() : 0;
    const auto cfg_scale = params.Has("cfgScale")? params.Get("cfgScale").As<Napi::Number>().FloatValue() : 7.0f;
    const auto guidance = params.Has("guidance")? params.Get("guidance").As<Napi::Number>().FloatValue() : 3.5f;
    const auto eta = params.Has("eta")? params.Get("eta").As<Napi::Number>().FloatValue() : 0.0f;
    const auto width = params.Has("width")? params.Get("width").As<Napi::Number>().Int32Value() : 512;
    const auto height = params.Has("height")? params.Get("height").As<Napi::Number>().Int32Value() : 512;
    const auto sample_method = params.Has("sampleMethod")? params.Get("sampleMethod").As<Napi::Number>().Int32Value() : 0;
    const auto sample_steps = params.Has("sampleSteps")? params.Get("sampleSteps").As<Napi::Number>().Int32Value() : 20;
    const auto seed = params.Has("seed")? params.Get("seed").As<Napi::Number>().Int32Value() : 0;
    const auto batch_count = params.Has("batchCount")? params.Get("batchCount").As<Napi::Number>().Int32Value() : 1;
    const auto style_ratio = params.Has("styleRatio")? params.Get("styleRatio").As<Napi::Number>().FloatValue() : 20.0f;
    const auto normalize_input = params.Has("normalizeInput")? params.Get("normalizeInput").As<Napi::Boolean>() : false;
    const auto input_id_images_path = params.Has("inputIdImagesPath")? params.Get("inputIdImagesPath").As<Napi::String>().Utf8Value() : "";
    const auto slg_scale = params.Has("slgScale")? params.Get("slgScale").As<Napi::Number>().FloatValue() : 0.0f;
    const auto skip_layer_start = params.Has("skipLayerStart")? params.Get("skipLayerStart").As<Napi::Number>().FloatValue() : 0.1f;
    const auto skip_layer_end = params.Has("skipLayerEnd")? params.Get("skipLayerEnd").As<Napi::Number>().FloatValue() : 0.2f;
    const auto skip_layers_array = params.Has("skipLayers")? params.Get("skipLayers").As<Napi::Array>() : Napi::Array::New(info.Env());
    std::vector<int> skip_layers;
    for (int i = 0; i < skip_layers_array.Length(); i++)
    {
        skip_layers.push_back(skip_layers_array.Get(i).As<Napi::Number>().Int32Value());
    }
    images = txt2img(context,
                           prompt.c_str(),
                           negativePrompt.c_str(),
                           clip_skip,
                           cfg_scale,
                           guidance,
                           eta,
                           width,
                           height,
                           static_cast<sample_method_t>(sample_method),
                           sample_steps,
                           seed,
                           batch_count,
                           nullptr,
                           0.9f,
                           style_ratio,
                           normalize_input,
                           input_id_images_path.c_str(),
                           skip_layers.data(),
                           skip_layers.size(),
                           slg_scale,
                           skip_layer_start,
                           skip_layer_end);

    imageCount = batch_count;
    return info.Env().Undefined();
}

Napi::Value StableDiffusion::SaveImageToPng(const Napi::CallbackInfo& info)
{
    const auto index = info[0].As<Napi::Number>().Int32Value();
    const auto output_path = info[1].As<Napi::String>().Utf8Value();
    const auto image = images[index];
    stbi_write_png(output_path.c_str(), image.width, image.height, 3, image.data, 0, "node-ml\n");
    return info.Env().Undefined();
}


