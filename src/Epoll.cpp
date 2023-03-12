#include "Epoll.h"

#define MAX_EVENTS 1024                     // 最大事件数

Epoll::Epoll() : epfd(-1), events(nullptr){
    epfd = epoll_create1(0);                // 创建一个epoll文件描述符
    errif(epfd == -1, "epoll创建错误!");   
    events = new epoll_event[MAX_EVENTS];   // 事件表
    bzero(events, sizeof(*events) * MAX_EVENTS);  // 初始化
}
Epoll::~Epoll(){
    if(epfd != -1){
        close(epfd);
        epfd = -1;
    }
    delete events;
}

void Epoll::addFd(int fd, uint32_t op){
    epoll_event ev;
    bzero(&ev, sizeof ev);
    ev.data.fd  = fd;
    ev.events   = op;

    errif( 
        epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev)==-1,
        "epoll添加服务错误!");
}
void Epoll::modFd(int fd, uint32_t op){
    epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd  = fd;
    ev.events   = op;
    
    errif( 
        epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev)==-1,
        "epoll修改服务错误!");
}

std::vector<epoll_event> Epoll::poll(int timeout){
    std::vector<epoll_event> activeEvents;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);   // 将内核中的事件复制到events中
    errif(nfds == -1, "epoll获取事件错误!");
    for(int i=0; i<nfds; i++)
        activeEvents.push_back(events[i]);  // 压入新的事件
    return activeEvents;
}