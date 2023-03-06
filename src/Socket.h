#pragma once
#include "Util.h"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

class InetAddress{
public:
    sockaddr_in6 addr;
    socklen_t addr_len;

    InetAddress(const char* ip, uint16_t port);
    ~InetAddress();
};

class Socket
{
private:
    int fd;
public:
    Socket();
    ~Socket();

    void bind(InetAddress*);    // socket 绑定IP
    void listen();              // 开始监听
    int accept(InetAddress*);   // 接受客户端
    
    void setnonblocking();      // 设置为非阻塞socket
    int getFd();                // 获取fd
};