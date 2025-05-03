//
// Created by denis.montes on 26/04/25.
//

#ifndef ADDON_ENCODEC_H
#define ADDON_ENCODEC_H
#include <napi.h>
#include "encodec.h"

class Encodec: public Napi::ObjectWrap<Encodec> {
public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Encodec(const Napi::CallbackInfo& info);
    ~Encodec();
    struct encodec_context * context;
};



#endif //ADDON_ENCODEC_H
