#include "Epoll.h"

#define MAX_EVENTS 10240                    // 最大事件数

Epoll::Epoll(MemoryPool* mp){
    epfd = epoll_create1(0);                // 创建一个epoll文件描述符
    err(epfd == -1, "epoll创建错误!");   
    events = new epoll_event[MAX_EVENTS];   // 事件表
    memset(events, '\0', MAX_EVENTS * sizeof *events);
    _mp = mp;
}
Epoll::~Epoll(){
    if(epfd != -1){
        close(epfd);
        epfd = -1;
    }
    delete []events;
}

void Epoll::addfd(int fd, uint32_t op){
    epoll_event ev;
    memset(&ev, 0, sizeof ev);
    ev.data.fd  = fd;
    ev.events   = op;

    err(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev)==-1,
        "epoll添加服务错误!");
}
void Epoll::updatech(Channel* ch){
    int fd = ch->fd();
    epoll_event ev;
    memset(&ev, 0, sizeof ev);
    ev.data.ptr = ch;
    ev.events = ch->events();

    if(!ch->inepoll()){
        err(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev)==-1,
            "epoll添加服务错误!");
        ch->setinepoll(true);
    } else
        err(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev)==-1,
            "epoll修改服务错误!");
}

std::vector<Channel*> Epoll::poll(int timeout){
    std::vector<Channel*> acevents;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);   // 将内核中的事件复制到events中
    err(nfds == -1, "epoll获取事件错误!");
    for(int i=0; i<nfds; i++){
        Channel* ch = (Channel*)events[i].data.ptr; 
        ch->setevents(events[i].events);
        acevents.push_back(ch);  // 压入新的事件
    }
    return acevents;
}