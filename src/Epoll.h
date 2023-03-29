#pragma once
#include "Util.h"
#include "Socket.h"
#include "MemoryPool.h"
#include "Channel.h"
#include <vector>
#include <string.h>
#include <sys/epoll.h>

class Channel;

class Epoll
{
public:
    Epoll(MemoryPool* mp);
    ~Epoll();

    // 添加事件到epoll
    void addfd(int fd, uint32_t op);
    // 更新事件到epoll
    void updatech(Channel* ch);
    // 获取内核事件表
    std::vector<Channel*> poll(int timeout);
private:
    MemoryPool* _mp; // 与之对应的内存池
    int epfd = -1;               
    epoll_event* events = NULL; // 事件表
};