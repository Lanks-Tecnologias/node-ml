//
// Created by denis.montes on 17/04/25.
//

#include <ggml-cpu.h>
#include <napi.h>
#include "ggml.h"
#include <memory>
#include <string>
#include <unordered_map>

class GGMLSession : public Napi::ObjectWrap<GGMLSession> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  explicit GGMLSession(const Napi::CallbackInfo& info);
  ~GGMLSession() override;

private:
  static Napi::FunctionReference constructor;
  struct ggml_init_params params;
  ggml_context* ctx = nullptr;

  Napi::Value RunAdd(const Napi::CallbackInfo& info);
};

Napi::FunctionReference GGMLSession::constructor;

Napi::Object GGMLSession::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "GGMLSession", {
    InstanceMethod("runAdd", &GGMLSession::RunAdd),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
  exports.Set("init", func);
  return exports;
}

GGMLSession::GGMLSession(const Napi::CallbackInfo& info) : Napi::ObjectWrap<GGMLSession>(info) {
  Napi::Env env = info.Env();
  auto config = info[0].As<Napi::Object>();
  std::string backend = config.Get("backend").As<Napi::String>();

  size_t mem_size = 16 * 1024 * 1024; // 16MB default
  void *mem_buffer = malloc(mem_size);

  this->params.mem_size = mem_size;
  this->params.mem_buffer = mem_buffer;
  this->params.no_alloc = false;

  this->ctx = ggml_init(this->params);

  if (!ctx) {
    Napi::TypeError::New(env, "Failed to initialize ggml context").ThrowAsJavaScriptException();
  }

  // Aqui você pode condicionar seleção de backend se tiver suporte na build do ggml
}

GGMLSession::~GGMLSession() {
  if (ctx) {
    ggml_free(ctx);
  }
  if (params.mem_buffer) {
    free(params.mem_buffer);
  }
}

Napi::Value GGMLSession::RunAdd(const Napi::CallbackInfo& info) {
  const float a = info[0].As<Napi::Number>().FloatValue();
  const float b = info[1].As<Napi::Number>().FloatValue();

  ggml_tensor* a_tensor = ggml_new_f32(ctx, a);
  ggml_tensor* b_tensor = ggml_new_f32(ctx, b);
  ggml_tensor* add = ggml_add(ctx, a_tensor, b_tensor);

  ggml_cgraph * gf = ggml_new_graph(ctx);
  ggml_build_forward_expand(gf, add);
  ggml_graph_compute_with_ctx(ctx, gf, 2);

  return Napi::Number::New(info.Env(), ggml_get_f32_1d(add, 0));
}
