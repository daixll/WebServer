#pragma once
#include <sys/epoll.h>
#include <vector>
#include "Util.h"
#include <unistd.h>
#include <string.h>

class Epoll
{
private:
    int epfd;
    struct epoll_event *events;
public:
    Epoll();
    ~Epoll();

    // 添加服务到epoll
    void addFd(int fd, uint32_t op);
    // 修改服务到epoll
    void modFd(int fd, uint32_t op);
    // 事件树
    std::vector<epoll_event> poll(int timeout=-1);
};