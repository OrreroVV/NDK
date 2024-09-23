//
// Created on 2024/9/11.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#include "hello.h"
#include <string>
#include <vector>
#include "httpRequest.h"
#include "hilog/log.h"
#include "hidebug/hidebug.h"
#include <thread>

#undef LOG_DOMAIN 
#undef LOG_TAG 
#define LOG_DOMAIN 0x3200 // 全局domain宏，标识业务领域 
#define LOG_TAG "testTag"  // 全局tag宏，标识模块日志tag

void runHttpRequest(std::string url) {
    HttpRequest httpRequest;
//     httpRequest.http_test();
    httpRequest.send_http_request(url, "", HttpRequest::RequestMode::GET, "");
}

napi_value get_URL(napi_env env, napi_callback_info info) {
     // 函数参数的个数
    size_t argc = 1;
    napi_value args[1];
    
    // 获取函数调用时传入的参数
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    
    double cpuUsage = OH_HiDebug_GetSystemCpuUsage();
    OH_LOG_INFO(LogType::LOG_APP, "GetSystemCpuUsage: %{public}f", cpuUsage);
    
     // 获取传入的字符串
    size_t str_len;
    napi_get_value_string_utf8(env, args[0], nullptr, 0, &str_len);
    
    char buffer[128];
    napi_get_value_string_utf8(env, args[0], buffer, sizeof(buffer), &str_len);
    
    std::string res = std::string(buffer);
    
    std::vector<std::pair<int, int>>h;
    OH_LOG_INFO(LogType::LOG_APP, "res: %{public}s", res.c_str());
    for (auto[k, v] : h) {
        
    }
    
     // 创建一个可分离的线程
    std::thread httpThread(runHttpRequest, res);
    // 分离线程，确保主线程不会阻塞
    httpThread.detach();

    // 主线程可以继续执行其他操作


    napi_value sum;
    napi_create_string_utf8(env, res.c_str(), res.size(), &sum);
    return sum;
}