//
// Created on 2024/9/23.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef TESTPROJECT_TESTVPN_H
#define TESTPROJECT_TESTVPN_H

class TestVPN {
public:
    TestVPN();
    ~TestVPN();
    void startVPN();
};

#endif //TESTPROJECT_TESTVPN_H
