#include "Socket.h"

InetAddress::InetAddress(const char *ip, uint16_t port){
    bzero(&addr, sizeof(addr));
    addr.sin6_family      = AF_INET6;           // IPV6 协议
    addr.sin6_port        = htons(port);        // IPV6 端口
    inet_pton(AF_INET6, ip, &addr.sin6_addr);   // IPV6 地址
    addr_len = sizeof(addr);
}
InetAddress::~InetAddress(){}

Socket::Socket() : fd(-1){  // 初始化fd为-1
    fd = socket(AF_INET6, SOCK_STREAM, 0);
    errif(fd == -1, "socket创建错误!");
}
Socket::~Socket(){
    if(fd != -1) close(fd);
    fd = -1;
}

void Socket::bind(InetAddress* addr){
    errif(
        ::bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1,
        "socket绑定错误!" );
}
void Socket::listen(){
    errif(
        ::listen(fd, SOMAXCONN) == -1,
        "socket监听错误!" );
}
int Socket::accept(InetAddress *addr){
    int clnt_sockfd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    errif(clnt_sockfd == -1, "socket接受错误!");
    return clnt_sockfd;
}
void Socket::setnonblocking(){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
int Socket::getFd(){
    return fd;
}