#pragma once
#include "Util.h"
#include <string.h>     // memset()
#include <unistd.h>     // close()
#include <fcntl.h>      // 设置非阻塞
#include <sys/types.h>  // 类型转换套件
#include <sys/socket.h> // socket套件
#include <arpa/inet.h>  // ip套件

class Socket
{
public:
    Socket(std::string IPV6="ipv6", std::string ip="[::]", int port=80);
    ~Socket();
    void setnonblocking();  // 此socket设置为非阻塞模式
    void online();          // 服务器上线运行
    void socket6(const char* ip, uint16_t port);    // ipv6 socket
    void socket4(const char* ip, uint16_t port);    // ipv4 socket
    int  ac6(sockaddr_in6 clnt_addr, socklen_t clnt_addr_len);
    int  ac4();             // 接受一个ipv4连接

    int             fd();    
    sockaddr_in     addr4();   
    sockaddr_in6    addr6();
    socklen_t       addr_len();
private:
    bool _IPV6;     // 仅仅支持ipv6, 如果false就是ipv4
    
    int             _fd;         // fd
    sockaddr_in     _addr;
    sockaddr_in6    _addr6;      // 地址( 协议 + ip + port )
    socklen_t       _addr_len;   // 地址大小
};