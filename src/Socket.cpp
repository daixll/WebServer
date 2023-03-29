#include "Socket.h"

void Socket::socket6(const char* ip, uint16_t port){
    memset(&_addr6, '\0', sizeof _addr6);
    _addr6.sin6_family = AF_INET6;              // IPV6 协议
    inet_pton(AF_INET6, ip, &_addr6.sin6_addr); // IPV6 地址
    _addr6.sin6_port = htons(port);             // IPV6 端口
    _addrlen = sizeof _addr6;
    fd = -1;
    fd = socket(AF_INET6, SOCK_STREAM, 0);      // 创建套接字
    err(fd==-1, "socket初始化错误!");
    return ;
}

void Socket::socket4(const char* ip, uint16_t port){
    // ipv4创建！！！
    return ;
}

Socket::Socket(char IPV, std::string ip, int port){
    _IPV = IPV;
    if(_IPV==6) socket6(ip.c_str(), port);
    else        socket4(ip.c_str(), port);

    int opt = 1;    // 端口复用
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&opt, sizeof opt);

    if(_IPV==6)
        err(bind(fd, (sockaddr*)&_addr6, _addrlen) == -1,
            "socket绑定错误!");
    else
        err(bind(fd, (sockaddr*)&_addr4, _addrlen) == -1,
            "socket绑定错误!");
}
Socket::~Socket(){
    if(fd != -1) close(fd);
    fd = -1;
}

void Socket::online(){
    err(listen(fd, SOMAXCONN) == -1, "socket监听错误!");
}

int Socket::ac6(sockaddr_in6 clntaddr, socklen_t clntaddrlen){
    int clntfd = accept(fd, (sockaddr*)&clntaddr, &clntaddrlen);
    err(clntfd == -1, "socket接受错误!");
    return clntfd;
}
int Socket::ac4(sockaddr_in clntaddr, socklen_t clntaddrlen){
    // 接受ipv4连接！！！
    return -1;
}

void Socket::setnonblocking(){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

sockaddr_in Socket::addr4(){
    return _addr4;
}
sockaddr_in6 Socket::addr6(){
    return _addr6;
}
socklen_t Socket::addrlen(){
    return _addrlen;
}