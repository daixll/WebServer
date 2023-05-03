#include "Socket.h"

void Socket::socket6(const char* ip, uint16_t port){
    _IPV6=1;
    memset(&_addr6, '\0', sizeof _addr6);
    _addr6.sin6_family = AF_INET6;               // IPV6 协议
    inet_pton(AF_INET6, ip, &_addr6.sin6_addr);  // IPV6 地址
    _addr6.sin6_port = htons(port);              // IPV6 端口
    _addr_len = sizeof _addr6;
    _fd = -1;
    _fd = socket(AF_INET6, SOCK_STREAM, 0);     // 创建套接字
    err(_fd==-1, 
        "socket初始化错误!");
    return ;
}
void Socket::socket4(const char* ip, uint16_t port){
}

Socket::Socket(std::string IPV6, std::string ip, int port){
    if(IPV6 == "ipv6")  socket6(ip.c_str(), port);
    else                socket4(ip.c_str(), port);

    int opt = 1;    // 端口复用
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&opt, sizeof(opt));

    err(bind(_fd, (sockaddr*)&_addr6, _addr_len) == -1,
        "socket绑定错误!");
}
Socket::~Socket(){
    if(_fd != -1) close(_fd);
    _fd = -1;
}

void Socket::online(){
    err(listen(_fd, SOMAXCONN) == -1,
        "socket监听错误!");
}
int Socket::ac6(sockaddr_in6 clnt_addr, socklen_t clnt_addr_len){
    int clntfd = accept(_fd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    err(clntfd == -1, 
        "socket接受错误!");
    return clntfd;
}
int Socket::ac4(){
    return -1;
}

void Socket::setnonblocking(){
    fcntl(_fd, F_SETFL, fcntl(_fd, F_GETFL) | O_NONBLOCK);
}

int Socket::fd(){
    return _fd;
}
sockaddr_in6 Socket::addr6(){
    return _addr6;
}
socklen_t Socket::addr_len(){
    return _addr_len;
}
