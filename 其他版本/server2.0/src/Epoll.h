#pragma once
#include "Util.h"
#include "Socket.h"
#include <vector>
#include <string.h>
#include <sys/epoll.h>
#include <map>

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
    std::vector<epoll_event> poll(int timeout=-1);

    std::map<int, std::string> fdfile;
};