//
// Created on 2024/9/11.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef TESTPROJECT_CUSSUM_H
#define TESTPROJECT_CUSSUM_H

#include "napi/native_api.h"
napi_value Add(napi_env env, napi_callback_info info);

#endif //TESTPROJECT_CUSSUM_H
