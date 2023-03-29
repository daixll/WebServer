#pragma once
#include "Util.h"
#include <string.h>     // memset()
#include <arpa/inet.h>  // ip套件
#include <sys/socket.h> // socket套件
#include <sys/types.h>  // 类型转换套件
#include <fcntl.h>      // 设置非阻塞
#include <unistd.h>     // close()

class Socket
{
public:
    Socket(char IPV=6, std::string ip="[::]", int port=80);
    ~Socket();

    void setnonblocking();  // 此socket设置为非阻塞模式
    void socket6(const char* ip, uint16_t port);    // ipv6 socket
    void socket4(const char* ip, uint16_t port);    // ipv4 socket
    void online();          // 服务器上线运行
    int  ac6(sockaddr_in6 clntaddr, socklen_t clntaddrlen); // 接受一个ipv6连接
    int  ac4(sockaddr_in clntaddr, socklen_t clntaddrlen);  // 接受一个ipv4连接

    int fd;     // servfd
    sockaddr_in     addr4();   
    sockaddr_in6    addr6();
    socklen_t       addrlen();
private:
    char            _IPV;       // 这个Socket的版本
    sockaddr_in     _addr4;
    sockaddr_in6    _addr6;     // 地址( 协议 + ip + port )
    socklen_t       _addrlen;   // 地址大小
};