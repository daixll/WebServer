#pragma once
#include "Socket.h"
#include "Epoll.h"
#include "MemoryPool.h"

class Epoll;

class Channel{
public:
    Channel(int fd, Epoll* ep, MemoryPool* mp);
    ~Channel();

    Epoll*  ep();   // 返回ep
    int     fd();   // 返回fd

    uint32_t events();              // 获取events
    void setevents(uint32_t ev);    // 设置events
    
    bool inepoll();             // 是否在ep中
    void setinepoll(bool st);   // 设置在ep中的状态
private:
    bool        _inep;  // 是否在epoll中

    int         _fd;
    uint32_t    _events;// 事件 
    Epoll*      _ep;
    MemoryPool* _mp;
    Block*      buf;    // file存放地
};