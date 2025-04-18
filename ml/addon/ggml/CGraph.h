//
// Created by denis.montes on 18/04/25.
//

#ifndef CGRAPH_H
#define CGRAPH_H
#include <ggml-impl.h>
#include <napi.h>


class CGraph: public Napi::ObjectWrap<CGraph> {
public:
    ggml_cgraph * graph = nullptr;
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);
    CGraph(const Napi::CallbackInfo& info);
    ~CGraph();
    Napi::Value GetSize(const Napi::CallbackInfo& info);
    Napi::Value GetNumberNodes(const Napi::CallbackInfo& info);
    Napi::Value GetNumberLeafs(const Napi::CallbackInfo& info);
    Napi::Value GetNodes(const Napi::CallbackInfo& info);
    Napi::Value GetGrads(const Napi::CallbackInfo& info);
    Napi::Value GetAccs(const Napi::CallbackInfo& info);
    Napi::Value GetLeafs(const Napi::CallbackInfo& info);
    Napi::Value GetVisitedHashSet(const Napi::CallbackInfo& info);
    Napi::Value GetOrder(const Napi::CallbackInfo& info);
};



#endif //CGRAPH_H
