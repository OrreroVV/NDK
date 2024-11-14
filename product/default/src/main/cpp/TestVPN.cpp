//
// Created on 2024/9/23.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#include <iostream>
#include <string>
#include <map>
#include "TestVPN.h"

TestVPN::TestVPN() {
    startVPN();
}
TestVPN::~TestVPN() {
    
}
void TestVPN::startVPN() {
     // 创建一个 VPN 连接
    
}