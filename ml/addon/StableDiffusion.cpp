//
// Created by denis.montes on 26/04/25.
//

#include "StableDiffusion.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "thirdparty/stb_image.h"

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#include "thirdparty/stb_image_write.h"

Napi::FunctionReference StableDiffusion::constructor;
Napi::FunctionReference Image::constructor;

Napi::Object Image::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Image", {
        InstanceAccessor("width", &Image::GetWidth, nullptr),
        InstanceAccessor("height", &Image::GetHeight, nullptr),
        InstanceAccessor("channels", &Image::GetChannels, nullptr),
        InstanceAccessor("pixelSize", &Image::GetPixelSize, nullptr),
        InstanceAccessor("data", &Image::GetData, nullptr),
        InstanceMethod("loadFromFile", &Image::LoadFromFile),
        InstanceMethod("saveToFile", &Image::SaveFile),
    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("Image", func);
    return exports;
}

Image::Image(const Napi::CallbackInfo& info): Napi::ObjectWrap<Image>(info)
{

}

Image::~Image()
{
}

Napi::Value Image::GetWidth(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), width);
}

Napi::Value Image::GetHeight(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), height);
}

Napi::Value Image::GetData(const Napi::CallbackInfo& info)
{
    // Caso o buffer não tenha sido inicializado
    if (buffer.IsEmpty()) {
        return info.Env().Undefined();
    }
    return buffer;

}

Napi::Value Image::GetChannels(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), static_cast<double>(channels.size()));
}

Napi::Value Image::GetPixelSize(const Napi::CallbackInfo& info)
{
    double pixelSize = 0;
    for (const auto & channel : channels)
    {
        pixelSize += channel.size;
    }
    return Napi::Number::New(info.Env(), pixelSize);
}

Napi::Value Image::LoadFromFile(const Napi::CallbackInfo& info)
{
    const auto path = info[0].As<Napi::String>().Utf8Value();
    int x,y, c;
    const auto data = stbi_load(path.c_str(),&x, &y, &c, 3);
    buffer = Napi::Buffer<uint8_t>::Copy(info.Env(), data, x * y * c);
    width = static_cast<u_int32_t>(x);
    height = static_cast<u_int32_t>(y);
    channels.resize(c);

    for (int i = 0; i < c; i++)
    {
        channels[i].size = 8;
        channels[i].index = i;
    }

    stbi_image_free(data);
    return Napi::Boolean::New(info.Env(), true);
}

Napi::Value Image::SaveFile(const Napi::CallbackInfo& info)
{
    const auto path = info[0].As<Napi::String>().Utf8Value();
    stbi_write_png(path.c_str(), width, height, 3, buffer.Data(), 0, "node-ml\n");
    return info.Env().Undefined();
}

Napi::Object StableDiffusion::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "StableDiffusion", {
        InstanceMethod("textToImage", &StableDiffusion::TextToImage),
        InstanceMethod("imageToImage", &StableDiffusion::ImageToImage),
        InstanceMethod("saveImageToPng", &StableDiffusion::SaveImageToPng)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    const auto sampleMethod = Napi::Object::New(env);
    sampleMethod.Set("EULER_A", static_cast<double>(sample_method_t::EULER_A));
    sampleMethod.Set("EULER", static_cast<double>(sample_method_t::EULER));
    sampleMethod.Set("HEUN", static_cast<double>(sample_method_t::HEUN));
    sampleMethod.Set("DPM2", static_cast<double>(sample_method_t::DPM2));
    sampleMethod.Set("DPMPP2S_A", static_cast<double>(sample_method_t::DPMPP2S_A));
    sampleMethod.Set("DPMPP2M", static_cast<double>(sample_method_t::DPMPP2M));
    sampleMethod.Set("DPMPP2Mv2", static_cast<double>(sample_method_t::DPMPP2Mv2));
    sampleMethod.Set("IPNDM", static_cast<double>(sample_method_t::IPNDM));
    sampleMethod.Set("IPNDM_V", static_cast<double>(sample_method_t::IPNDM_V));
    sampleMethod.Set("LCM", static_cast<double>(sample_method_t::LCM));
    sampleMethod.Set("DDIM_TRAILING", static_cast<double>(sample_method_t::DDIM_TRAILING));
    sampleMethod.Set("TCD", static_cast<double>(sample_method_t::TCD));
    sampleMethod.Set("N_SAMPLE_METHODS", static_cast<double>(sample_method_t::N_SAMPLE_METHODS));
    exports.Set("SampleMethod", sampleMethod);

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
    const auto vaeDecodeOnly = options.Has("vaeDecodeOnly")? options.Get("vaeDecodeOnly").As<Napi::Boolean>() : false;
    context = new_sd_ctx(modelPath.c_str(), clip_l_Path.c_str(), clip_g_Path.c_str(), "",
    diffusionModelPath.c_str(), vaePath.c_str(), taePath.c_str(), ctrNetPath.c_str(),
        loraPath.c_str(), embedDir.c_str(), "", vaeDecodeOnly, true, false, 16, sd_type_t::SD_TYPE_Q4_0,
        rng_type_t::STD_DEFAULT_RNG, DEFAULT, false, true, false, true, device
    )
    ;
}

StableDiffusion::~StableDiffusion()
{
    if (images != nullptr && imageCount > 0)
        free(images);

    if (context != nullptr)
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
    const auto imageArray = Napi::Array::New(info.Env(), imageCount);

    for (int i = 0; i < imageCount; i++)
    {
        const auto imageObj = Image::constructor.New({});
        const auto image = Image::Unwrap(imageObj);
        image->height = height;
        image->width = width;
        image->buffer = Napi::Buffer<uint8_t>::Copy(info.Env(), images[i].data, width * height * images[i].channel);
        imageArray.Set(i, imageObj);
    }

    return imageArray;
    // return info.Env().Undefined();
}

Napi::Value StableDiffusion::SaveImageToPng(const Napi::CallbackInfo& info)
{
    const auto index = info[0].As<Napi::Number>().Int32Value();
    const auto output_path = info[1].As<Napi::String>().Utf8Value();
    const auto image = images[index];
    stbi_write_png(output_path.c_str(), image.width, image.height, 3, image.data, 0, "node-ml\n");
    return info.Env().Undefined();
}

Napi::Value StableDiffusion::ImageToImage(const Napi::CallbackInfo& info)
{
    const auto options = info[0].As<Napi::Object>();

    if (!options.Has("initImage"))
    {
        Napi::TypeError::New(info.Env(), "Missing initImage").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    if (!options.Has("maskImage"))
    {
        Napi::TypeError::New(info.Env(), "Missing maskImage").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    const auto initImageObj = options.Get("initImage").As<Napi::Object>();
    const auto initImageWrap = Image::Unwrap(initImageObj);
    sd_image_t initImage = sd_image_t();
    initImage.width = initImageWrap->width;
    initImage.height = initImageWrap->height;
    initImage.channel = initImageWrap->channels.size();
    initImage.data = initImageWrap->buffer.Data();
    auto lenght = initImageWrap->buffer.Length();

    //@TODO Load mask from params
    sd_image_t maskImage = sd_image_t();
    maskImage.width = initImageWrap->width;
    maskImage.height = initImageWrap->height;
    maskImage.channel = initImage.channel;
    maskImage.data = static_cast<uint8_t*>(malloc(lenght));
    for (int i = 0; i < lenght; i++)
    {
        maskImage.data[i] = 0xFF;
    }

    if (!options.Has("prompt"))
    {
        Napi::TypeError::New(info.Env(), "Missing prompt").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    const auto prompt = options.Get("prompt").As<Napi::String>().Utf8Value();

    if (!options.Has("negativePrompt"))
    {
        Napi::TypeError::New(info.Env(), "Missing prompt").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }
    const auto negativePrompt = options.Get("negativePrompt").As<Napi::String>().Utf8Value();
    const auto clip_skip = options.Has("clipSkip")? options.Get("clipSkip").As<Napi::Number>().Int32Value() : 0;
    const auto cfg_scale = options.Has("cfgScale")? options.Get("cfgScale").As<Napi::Number>().FloatValue() : 7.0f;
    const auto guidance = options.Has("guidance")? options.Get("guidance").As<Napi::Number>().FloatValue() : 3.5f;
    const auto eta = options.Has("eta")? options.Get("eta").As<Napi::Number>().FloatValue() : 0.0f;
    const auto width = options.Has("width")? options.Get("width").As<Napi::Number>().Int32Value() : 512;
    const auto height = options.Has("height")? options.Get("height").As<Napi::Number>().Int32Value() : 512;
    const auto strength = options.Has("strength")? options.Get("strength").As<Napi::Number>().FloatValue() : 1;
    const auto sample_method = options.Has("sampleMethod")? options.Get("sampleMethod").As<Napi::Number>().Int32Value() : 0;
    const auto sample_steps = options.Has("sampleSteps")? options.Get("sampleSteps").As<Napi::Number>().Int32Value() : 20;
    const auto seed = options.Has("seed")? options.Get("seed").As<Napi::Number>().Int32Value() : 0;
    const auto batch_count = options.Has("batchCount")? options.Get("batchCount").As<Napi::Number>().Int32Value() : 1;
    const auto style_strength = options.Has("styleStrength")? options.Get("styleStrength").As<Napi::Number>().FloatValue() : 20.0f;
    const auto normalize_input = options.Has("normalizeInput")? options.Get("normalizeInput").As<Napi::Boolean>() : false;
    const auto input_id_images_path = options.Has("inputIdImagesPath")? options.Get("inputIdImagesPath").As<Napi::String>().Utf8Value() : "";
    const auto slg_scale = options.Has("slgScale")? options.Get("slgScale").As<Napi::Number>().FloatValue() : 0.0f;
    const auto skip_layer_start = options.Has("skipLayerStart")? options.Get("skipLayerStart").As<Napi::Number>().FloatValue() : 0.1f;
    const auto skip_layer_end = options.Has("skipLayerEnd")? options.Get("skipLayerEnd").As<Napi::Number>().FloatValue() : 0.2f;
    const auto skip_layers_array = options.Has("skipLayers")? options.Get("skipLayers").As<Napi::Array>() : Napi::Array::New(info.Env());
    std::vector<int> skip_layers;
    for (int i = 0; i < skip_layers_array.Length(); i++)
    {
        skip_layers.push_back(skip_layers_array.Get(i).As<Napi::Number>().Int32Value());
    }


    //const auto maskImage = initImage.Get("maskImage").As<Napi::Object>();
    const auto result = img2img(context, initImage, maskImage,prompt.c_str(), negativePrompt.c_str(),clip_skip, cfg_scale, guidance, eta, width, height,static_cast<sample_method_t>(sample_method), sample_steps, strength, seed, batch_count, nullptr, 0.9f, style_strength, normalize_input, input_id_images_path.c_str(), skip_layers.data(), skip_layers.size(), slg_scale, skip_layer_start, skip_layer_end);

    imageCount = batch_count;
    const auto imageArray = Napi::Array::New(info.Env(), imageCount);

    for (int i = 0; i < imageCount; i++)
    {
        const auto imageObj = Image::constructor.New({});
        const auto image = Image::Unwrap(imageObj);
        image->height = height;
        image->width = width;
        image->buffer = Napi::Buffer<uint8_t>::Copy(info.Env(), result[i].data, width * height * result[i].channel);
        imageArray.Set(i, imageObj);
    }

    return imageArray;

}


