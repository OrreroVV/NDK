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
#include "zconf.h"
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


#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <cstring>
#include <fcntl.h>
#include <netinet/ip.h>

#define PORT 12345
#define MAX_EVENTS 10
#define BUFFER_SIZE 4096

// 设置套接字为非阻塞
void setSocketNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

// 创建 TCP 套接字
int createTcpSocket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);  // 使用 SOCK_STREAM 创建 TCP 套接字
    if (sockfd < 0) {
        OH_LOG_ERROR(LogType::LOG_APP, "Failed to create socket");
        exit(1);
    }
    OH_LOG_INFO(LogType::LOG_APP, "Socket created successfully");
    return sockfd;
}

// 绑定套接字到端口
void bindSocketToPort(int sockfd) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // 监听所有 IP 地址
    server_addr.sin_port = htons(PORT);  // 绑定到端口 14400

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        OH_LOG_ERROR(LogType::LOG_APP, "Failed to bind socket");
        exit(1);
    }
    OH_LOG_INFO(LogType::LOG_APP, "Socket bound to port %d", PORT);
}

// 设置监听
void setupListen(int sockfd) {
    if (listen(sockfd, 5) < 0) {
        OH_LOG_ERROR(LogType::LOG_APP, "Failed to listen on socket");
        exit(1);
    }
    OH_LOG_INFO(LogType::LOG_APP, "Listening on socket");
}

// 接收客户端数据
void receiveData(int sockfd) {
    char buffer[BUFFER_SIZE];
    int bytesReceived = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytesReceived < 0) {
        if (errno != EAGAIN) {  // EAGAIN 表示数据未准备好，可以继续等待
            OH_LOG_ERROR(LogType::LOG_APP, "Failed to receive data");
        }
    } else if (bytesReceived == 0) {
        OH_LOG_INFO(LogType::LOG_APP, "Client disconnected");
        close(sockfd);  // 客户端关闭连接
    } else {
        OH_LOG_INFO(LogType::LOG_APP, "Received data: %{public}.*s", bytesReceived, buffer);
    }
}

// 处理 epoll 事件
void handleEpollEvent(int epollfd, int listenfd, struct epoll_event *event) {
    OH_LOG_ERROR(LogType::LOG_APP, "event: %{public}d", event->data.fd);
    if (event->events & EPOLLIN) {
        if (event->data.fd == listenfd) {
            // 有新连接请求到来
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int client_sock = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
            if (client_sock < 0) {
                OH_LOG_ERROR(LogType::LOG_APP, "Failed to accept connection");
                return;
            }

            // 设置新连接套接字为非阻塞
            setSocketNonBlocking(client_sock);

            // 将新连接加入 epoll 实例
            struct epoll_event client_event;
            client_event.events = EPOLLIN | EPOLLET;  // 使用边缘触发
            client_event.data.fd = client_sock;
            if (epoll_ctl(epollfd, EPOLL_CTL_ADD, client_sock, &client_event) < 0) {
                OH_LOG_ERROR(LogType::LOG_APP, "Failed to add client socket to epoll");
                close(client_sock);
            }

            OH_LOG_INFO(LogType::LOG_APP, "New client connected");
        } else {
            // 有数据可读
            receiveData(event->data.fd);
        }
    }
}


void listenRawSocket() {
    // 创建 TCP 套接字
    int sockfd = createTcpSocket();

    // 绑定套接字到端口 14400
    bindSocketToPort(sockfd);

    // 设置套接字开始监听
    setupListen(sockfd);

    // 设置套接字为非阻塞
    setSocketNonBlocking(sockfd);

    // 创建 epoll 实例
    int epollfd = epoll_create1(0);
    if (epollfd < 0) {
        OH_LOG_ERROR(LogType::LOG_APP, "Failed to create epoll instance");
        exit(1);
    }

    OH_LOG_INFO(LogType::LOG_APP, "Epoll instance created");

    // 将监听套接字添加到 epoll 实例中
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;  // 设置为边缘触发
    event.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event) < 0) {
        OH_LOG_ERROR(LogType::LOG_APP, "Failed to add socket to epoll");
        exit(1);
    }

    OH_LOG_INFO(LogType::LOG_APP, "Added socket to epoll");

    // 设置 epoll 等待事件
    struct epoll_event events[MAX_EVENTS];
    while (true) {
        int numEvents = epoll_wait(epollfd, events, MAX_EVENTS, -1);  // 阻塞等待事件
        if (numEvents < 0) {
            OH_LOG_ERROR(LogType::LOG_APP, "epoll_wait failed");
            exit(1);
        }

        OH_LOG_INFO(LogType::LOG_APP, "epoll_wait returned %{public}d events", numEvents);

        for (int i = 0; i < numEvents; ++i) {
            handleEpollEvent(epollfd, sockfd, &events[i]);
        }
    }

    // 清理
    OH_LOG_INFO(LogType::LOG_APP, "Cleaning up and closing sockets");
    close(sockfd);
    close(epollfd);

}

napi_value native_setFd(napi_env env, napi_callback_info info) {
    // 函数参数的个数
    size_t argc = 1;
    napi_value args[1];
    // 获取函数调用时传入的参数
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
     // 读取传入的 number 类型参数
    int32_t fd;
    napi_get_value_int32(env, args[0], &fd);
    OH_LOG_INFO(LogType::LOG_APP, "native tunFd: %{public}d", fd);
     // 创建一个新线程来执行 listenRawSocket 函数
    std::thread listenerThread(listenRawSocket);

    // 在主线程中执行其他任务，或者等待监听线程完成
    listenerThread.detach();
    return NULL;
}