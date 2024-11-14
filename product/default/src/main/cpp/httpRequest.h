//
// Created on 2024/9/12.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef TESTPROJECT_HTTPREQUEST_H
#define TESTPROJECT_HTTPREQUEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <openssl/ssl.h>
#include <openssl/err.h>

class HttpRequest {
public:
    enum RequestMode {
    GET, POST, PUT, DELETE
    };
    HttpRequest();
    ~HttpRequest();
    int create_socket(const std::string& address, int port);
    void send_http_request(std::string url, std::string cookie, RequestMode mode, std::string body);
    void http_test();
private:
    int sockfd;
    int MAX_TIMEOUT_S;
};


#endif //TESTPROJECT_HTTPREQUEST_H
