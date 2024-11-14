//
// Created on 2024/9/11.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef TESTPROJECT_HELLO_H
#define TESTPROJECT_HELLO_H
#include <vector>
#include <string>
#include <iostream>
#include "napi/native_api.h"

napi_value get_URL(napi_env env, napi_callback_info info);
napi_value native_setFd(napi_env env, napi_callback_info info);

#endif //TESTPROJECT_HELLO_H
