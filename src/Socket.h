#pragma once
#include "Util.h"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <cstring>

class Sock
{
public:
    Sock(const char* ip, uint16_t port);    // 初始化
    ~Sock();

    void    online();           // 服务上线
    int     start(sockaddr_in6 clnt_addr, socklen_t clnt_addr_len); // 服务运行

    void    setnonblocking();   // 此socket设置为非阻塞模式
    
    int             fd;         // 获取标识符
    sockaddr_in6    addr();     // 地址( 协议 + ip + port )
    socklen_t       addr_len(); // 地址大小
private:
    void    iport(const char* ip, uint16_t port);

    sockaddr_in6    _addr;       // 地址( 协议 + ip + port )
    socklen_t       _addr_len;   // 地址大小
};