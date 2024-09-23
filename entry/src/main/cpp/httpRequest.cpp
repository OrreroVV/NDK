//
// Created on 2024/9/12.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#include "httpRequest.h"
#include <iostream>
#include <netdb.h>
#include <regex> 
#include <fcntl.h>
#include <cstdlib>
#include "hilog/log.h"

#undef LOG_DOMAIN 
#undef LOG_TAG 
#define LOG_DOMAIN 0x3200 // 全局domain宏，标识业务领域 
#define LOG_TAG "testTag"  // 全局tag宏，标识模块日志tag

HttpRequest::HttpRequest() 
    :MAX_TIMEOUT_S(10) {
    
}

HttpRequest::~HttpRequest() {
    
}

// 判断是否是有效的 IP 地址
bool is_valid_ip(const std::string& str) {
    if (str.empty()) return false;

    struct sockaddr_in sa;
    struct sockaddr_in6 sa6;

    if (inet_pton(AF_INET, str.c_str(), &(sa.sin_addr)) == 1) {
        return true;
    }


    return false;
}

    // 解析域名为 IP 地址
int resolve_hostname(const std::string& hostname, std::string& ans) {
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET6 for IPv6, AF_INET for IPv4
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname.c_str(), NULL, &hints, &res)) != 0) {
        
        return -1;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        std::string ipver;

        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        std::string ip_address(ipstr);

        freeaddrinfo(res); // Free the linked list
        ans = ip_address;
        return 1;
    }

    freeaddrinfo(res); // Free the linked list
    return 0;
}

int parse_url(const std::string &url, std::string &host, std::string &port, std::string &path) {
    // 使用正则表达式解析 URL
    std::regex url_regex(R"(^(\w+):\/\/([^\/:]+)(:(\d+))?\/?(.*)$)");
    std::smatch url_match_result;

    if (std::regex_match(url, url_match_result, url_regex)) {
        std::string protocol = url_match_result[1];
        host = url_match_result[2];
        port = url_match_result[4];  // 可选的端口号
        path = url_match_result[5];
        if (path.size() && path[0] != '/') path = '/' + path;
        else if (path.empty()) path = '/';
        // 如果没有指定端口，根据协议设置默认端口
        if (port.empty()) {
            if (protocol == "https") {
                port = "443";
            } else if (protocol == "http") {
                port = "80";
            } else {
                port = "80";
            }
        }
    } else {
        return -1;
    }
    return 0;
}

int HttpRequest::create_socket(const std::string& address, int port) {
        struct sockaddr_in server_addr;

        // 创建 socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            return -1;
        }

        OH_LOG_INFO(LogType::LOG_APP, "create_socket success");
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);

        // 解析 IP 地址或域名
        std::string ip_address;
        if (is_valid_ip(address)) {
            ip_address = address;
        } else {
            int ret = resolve_hostname(address, ip_address);
            if (ret <= 0 || ip_address.empty()) {
                
                OH_LOG_INFO(LogType::LOG_APP, "resolve_hostname failed, hostname: %{public}s", strerror(errno));
                return -1;
            }
        }
        
        OH_LOG_INFO(LogType::LOG_APP, "resolve_hostname success%{public}s", ip_address.c_str());

        // 将 IP 地址转换为网络字节序
        if (inet_pton(AF_INET, ip_address.c_str(), &server_addr.sin_addr) <= 0) {
            OH_LOG_INFO(LogType::LOG_APP, "Invalid address or address not supported");
            return -1;
        }

        OH_LOG_INFO(LogType::LOG_APP, "inet_pton success");
        //  非阻塞IO
        fcntl(sockfd, F_SETFL, O_NONBLOCK);

            // Start connection attempt
        int result = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
        if (result < 0) {
        
            OH_LOG_INFO(LogType::LOG_APP, "connect failed: %{public}s", strerror(errno));
            if (errno != EINPROGRESS) {
                return -1; // Immediate failure
            }

            fd_set write_fds;
            fd_set error_fds;
            struct timeval tv;

            FD_ZERO(&write_fds);
            FD_ZERO(&error_fds);
            FD_SET(sockfd, &write_fds);
            FD_SET(sockfd, &error_fds);

            tv.tv_sec = MAX_TIMEOUT_S;
            tv.tv_usec = 0;

            // Wait for socket to be ready or timeout
            result = select(sockfd + 1, nullptr, &write_fds, &error_fds, &tv);

            if (result == 0) {
                // Timeout
                return -2;
            } else if (result < 0) {
                // Error
                return -1;
            }

            if (FD_ISSET(sockfd, &error_fds)) {
                // Error during connection
                return -1;
            }
        }
        return sockfd;
    }

int uy_sslconnect_until(int fd, SSL *ssl)
    {
        fd_set rset, wset, eset;
        int err, sslerr;
        struct timeval tv;

        FD_ZERO(&rset);
        FD_ZERO(&wset);
        FD_ZERO(&eset);

        while (true) {
            err = SSL_connect(ssl);
            if (err > 0)
                return 0;
            sslerr = SSL_get_error(ssl, err);
        
            OH_LOG_INFO(LogType::LOG_APP, "SSL_get_error: %{public}d", sslerr);
            switch (sslerr) {
                case SSL_ERROR_WANT_READ:
//                     FD_CLR(fd, &wset);
//                     FD_SET(fd, &rset); 
//                     FD_SET(fd, &eset);
                    break;

                case SSL_ERROR_WANT_WRITE:
                    FD_CLR(fd, &rset);
                    FD_SET(fd, &wset);
                    FD_SET(fd, &eset);
                    break;

                default:
                    return -1;
            }
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            err = select(fd + 1, &rset, &wset, &eset, &tv);
            if (err < 0) {
                return -1;
            }
        }

        return 0;
    }


void HttpRequest::send_http_request(std::string url, std::string cookie, RequestMode mode, std::string body) {
    // Implementation of sending HTTP requests using N-API
    // For example, you can use libcurl or other libraries to make HTTP requests
//     url = "https://www.baidu.com";
    
    OH_LOG_INFO(LogType::LOG_APP, "send_http_request");
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    
    SSL_CTX* ctx = SSL_CTX_new(SSLv23_client_method());
    if (ctx == nullptr) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    
    OH_LOG_INFO(LogType::LOG_APP, "SSL_CTX_new success");
    
    // TODO no prase ipv6 
    std::string host, port, path;
    if (parse_url(url, host, port, path) < 0) {
        
        OH_LOG_INFO(LogType::LOG_APP, "parse_url failed");
        return;
    }

    try {
        int value = std::stoi(port);
    } catch (...) {
        return;
    }

    OH_LOG_INFO(LogType::LOG_APP, "parse_url success");
    
// Create and connect the socket
    sockfd = create_socket(host, std::stoi(port));
    if (sockfd < 0) {
        return;
    }    
    
    OH_LOG_INFO(LogType::LOG_APP, "connect_socket success");
//     SSL_CTX_set_options(ctx, SSL_OP_NO_TLSv1 | SSL_OP_NO_TLSv1_1 | SSL_OP_NO_TLSv1_2 |  SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);
    
    SSL_CTX_set_cipher_list(ctx, "ALL:!ECDHE-SM4-SM3:!ECC-SM1-SM3:!ECDHE-SM1-SM3"); // ECC-SM4-SM3  ALL:!ECDHE-SM4-SM3:!ECC-SM1-SM3:!ECDHE-SM1-SM3
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);
    int ret = uy_sslconnect_until(sockfd, ssl); // 调用 SSL_connect
    if (ret < 0) {
        int err = SSL_get_error(ssl, ret);
        const char* errorString = ERR_error_string(ERR_get_error(), nullptr);
        OH_LOG_INFO(LogType::LOG_APP, "SSL connect failed: %{public}s, error code: %{public}d", errorString, err);
    } else {

 std::string requestHeadFmtStr = 
        "%s %s HTTP/1.1\r\n"
        "Content-Type: application/json;charset=\"UTF-8\"\r\n"
        "User-Agent: linux\r\n"
        "Host: %s:%d\r\n"
        "Cookie: %s\r\n"
        "Content-Length: %d\r\n\r\n%s";

    // 构建 HTTP 请求
    std::string ip = host;
    char buffer_send[1024];
    snprintf(buffer_send, sizeof(buffer_send), requestHeadFmtStr.c_str(), 
             "GET", path.c_str(), ip.c_str(), 443, "", 0, "");
        SSL_write(ssl, buffer_send, strlen(buffer_send));

        fd_set rset, wset, eset;
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        FD_ZERO(&eset);
        
        FD_SET(sockfd, &rset); // 监控读事件
        FD_SET(sockfd, &eset); // 监控异常事件
        
        int max_fd = sockfd; // 假设 fd 是最大文件描述符
        struct timeval timeout;
        timeout.tv_sec = 5; // 设置超时为5秒
        timeout.tv_usec = 0;
        
        int activity = select(max_fd + 1, &rset, &wset, &eset, &timeout);
        if (activity < 0) {
            OH_LOG_ERROR(LogType::LOG_APP, "Select error: %{public}s", strerror(errno));
        } else if (activity == 0) {
            OH_LOG_INFO(LogType::LOG_APP, "Select timeout");
        } else {
            // 检查读事件
            if (FD_ISSET(sockfd, &rset)) {
                // 使用 SSL_read 接收数据
                char buffer_read[2048];
                int bytes = SSL_read(ssl, buffer_read, sizeof(buffer_read));
                
                if (bytes > 0) {
                    buffer_read[bytes] = '\0'; // 确保字符串以空字符结尾
                    OH_LOG_INFO(LogType::LOG_APP, "Received %{public}s, size: %{public}d", buffer_read, bytes);
                } else {
                    int sslerr = SSL_get_error(ssl, bytes);
                    OH_LOG_ERROR(LogType::LOG_APP, "SSL_read error: %{public}d", sslerr);
                }
            }
        
            // 检查异常事件
            if (FD_ISSET(sockfd, &eset)) {
                OH_LOG_ERROR(LogType::LOG_APP, "Exception on socket");
            }
        }
    }

    SSL_free(ssl);
    close(sockfd);
    SSL_CTX_free(ctx);
}

void HttpRequest::http_test() {
//     func1();
//     return;
    const char *serverIp = "180.101.50.188"; // 替换为目标服务器IP
    const int serverPort = 443; // 替换为目标服务器端口


     // 创建 socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        OH_LOG_INFO(LogType::LOG_APP, "Socket creation failed");
        return;
    }

    // 设置 socket 为非阻塞
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    // 服务器地址结构
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIp, &serverAddr.sin_addr);

    // 尝试连接
    int connectStatus = connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (connectStatus < 0 && errno != EINPROGRESS) {
        OH_LOG_INFO(LogType::LOG_APP, "Connection failed");
        close(sock);
        return;
    }

    // 使用 select 管理连接
    fd_set writeSet;
    struct timeval timeout;
    timeout.tv_sec = 5; // 设置超时为5秒
    timeout.tv_usec = 0;

    FD_ZERO(&writeSet);
    FD_SET(sock, &writeSet);

    // 等待 socket 可写
    int activity = select(sock + 1, nullptr, &writeSet, nullptr, &timeout);
    if (activity < 0) {
        OH_LOG_INFO(LogType::LOG_APP, "Select error");
        close(sock);
        return;
    } else if (activity == 0) {
        OH_LOG_INFO(LogType::LOG_APP, "Connection timeout");
        close(sock);
        return;
    }

    // 检查连接状态
    int so_error;
    socklen_t len = sizeof(so_error);
    if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len) < 0) {
        OH_LOG_INFO(LogType::LOG_APP, "getsockopt error");
        close(sock);
        return;
    }

    if (so_error != 0) {
        OH_LOG_INFO(LogType::LOG_APP, "Connection failed: %s", strerror(so_error));
        close(sock);
        return;
    }

    OH_LOG_INFO(LogType::LOG_APP, "connect_socket success");
    // 构建 HTTP 请求
    std::string ip = serverIp;
    std::string message = "GET / HTTP/1.1\r\nHost:" + ip + "\r\nConnection: close\r\n\r\n"; // 替换为目标服务器的 IP 地址
    send(sock, message.c_str(), message.size(), 0);

    // 接收响应
    char buffer[2000];
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(sock, &readSet);

    timeout.tv_sec = 5; // 设置超时为5秒
    timeout.tv_usec = 0;

    activity = select(sock + 1, &readSet, nullptr, nullptr, &timeout);

    if (activity < 0) {
        OH_LOG_ERROR(LogType::LOG_APP, "Select error during recv");
        close(sock);
        return;
    } else if (activity == 0) {
        OH_LOG_ERROR(LogType::LOG_APP, "recv timeout");
        close(sock);
        return;
    } else {
        int received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (received > 0) {
            buffer[received] = '\0'; // 确保字符串以空字符结尾
            OH_LOG_INFO(LogType::LOG_APP, "recv: %{public}s", buffer);
        } else if (received == 0) {
            OH_LOG_WARN(LogType::LOG_APP, "Connection closed by peer");
        } else {
            OH_LOG_ERROR(LogType::LOG_APP, "recv failed: %{public}s", strerror(errno));
        }
    }
    // 关闭 socket
    close(sock);
}