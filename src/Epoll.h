#pragma once
#include <sys/epoll.h>
#include <vector>
#include "Util.h"
#include <unistd.h>
#include <string.h>
#include <string>
#include <map>

struct req{
    std::string method; // 请求的方法
    std::string file;   // 请求的文件
    std::string httpv;  // HTTP版本
    std::map<std::string, std::string> head;    // 各种标头
    std::string post;   // post内容
};

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
    // 查询fd对应的文件
    std::map<int, req> request;    // 需要手动释放内存
};