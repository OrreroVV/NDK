//
// Created on 2024/9/11.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#include "hello.h"
#include <string>
#include <vector>

napi_value get_URL(napi_env env, napi_callback_info info) {
     // 函数参数的个数
    size_t argc = 1;
    napi_value args[1];
    
    // 获取函数调用时传入的参数
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    
     // 获取传入的字符串
    size_t str_len;
    napi_get_value_string_utf8(env, args[0], nullptr, 0, &str_len);
    
    char buffer[128];
    napi_get_value_string_utf8(env, args[0], buffer, sizeof(buffer), &str_len);
    
    napi_value sum;
    std::string res = "sss" + std::string(buffer);
    
    std::vector<std::pair<int, int>>h;
    for (auto[k, v] : h) {
        
    }
    
    napi_create_string_utf8(env, res.c_str(), res.size(), &sum);
    return sum;
}