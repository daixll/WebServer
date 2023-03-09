#include "Socket.h"

void Sock::iport(const char* ip, uint16_t port){
    bzero(&_addr, sizeof _addr);
    _addr.sin6_family = AF_INET6;                // IPV6 协议
    inet_pton(AF_INET6, ip, &_addr.sin6_addr);   // IPV6 地址
    _addr.sin6_port = htons(port);               // IPV6 端口
    _addr_len = sizeof _addr;
}

Sock::Sock(const char* ip, uint16_t port){
    iport(ip, port);
    fd = -1;                                // 初始化 private 变量
    fd = socket(AF_INET6, SOCK_STREAM, 0);  // 创建套接字
    errif(  fd==-1, 
            "socket初始化错误!" );

    int opt = 1;    // 端口复用
	setsockopt( fd, SOL_SOCKET,SO_REUSEADDR, (const void *)&opt, sizeof(opt) );

    errif(  bind(fd, (sockaddr*)&_addr, _addr_len) == -1,
            "socket绑定错误!"   );
}
Sock::~Sock(){
    if(fd != -1) close(fd);
    fd = -1;
}

void Sock::online(){
    errif(  listen(fd, SOMAXCONN) == -1,
            "socket监听错误!"   );
}
int Sock::start(sockaddr_in6 clnt_addr, socklen_t clnt_addr_len){
    int clntfd = accept(fd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    errif(  clntfd == -1, 
            "socket接受错误!"   );
    return clntfd;
}

void Sock::setnonblocking(){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

sockaddr_in6 Sock::addr(){
    return _addr;
}
socklen_t Sock::addr_len(){
    return _addr_len;
}