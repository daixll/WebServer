#pragma once
#include "Util.h"
#include "Socket.h"
#include "MemoryPool.h"
#include "Channel.h"
#include <vector>
#include <string.h>
#include <sys/epoll.h>

class Epoll
{
private:
    int epfd;               
    epoll_event* events;
public:
    Epoll();
    ~Epoll();

    // 添加事件到epoll
    void addFd(int fd, uint32_t op);
    // 修改事件到epoll
    void modFd(int fd, uint32_t op);
    // 获取内核事件表
    std::vector<Channel*> Epoll::poll(int timeout, MemoryPool* mp);
    //std::vector<std::pair<epoll_event>> poll(int timeout=-1);
};