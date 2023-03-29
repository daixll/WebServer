#pragma once
#include "Socket.h"
#include "Epoll.h"
#include "MemoryPool.h"

class Channel{
public:
    Channel(int fd, Epoll* ep, MemoryPool* mp);
    ~Channel();
    
    epoll_event ev;

    void needsend(std::string& file);   // 需要发送的文件, 写入内存池
    char* sendfile(int size);     // 返回size大小的文件(如果有的话)
private:
    long long last=0;   // 上次发送文件的结束位置
    Epoll*      _ep;
    MemoryPool* _mp;
    Block*      buf;    // file存放地
};