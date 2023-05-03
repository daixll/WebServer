#include "Acceptor.h"

void loop(Socket* serv, Epoll* ep, ThreadPool* tp){
    while(true){
        std::vector<epoll_event> events = ep->poll(1000);    // 就绪事件
        
        for(int i=0; i<events.size(); i++)
            if(events[i].data.fd == serv->fd()){  // 新的事件
                
                int clnt = socket(AF_INET6, SOCK_STREAM, 0);     // 创建套接字
                sockaddr_in6 addr; int addr_len = sizeof addr;  // ip相关
                clnt = serv->ac6(addr, addr_len);
                fcntl(clnt, F_SETFL, fcntl(clnt, F_GETFL) | O_NONBLOCK);

                printf("\n新的连接%d |", clnt);

                ep->addFd(clnt, EPOLLIN | EPOLLET);
            }
            else if(events[i].events & EPOLLIN){// 可读事件
                function<void()> f = std::bind(dealrecv, (int)events[i].data.fd, ep);
                tp -> AddTask(f);
            }
            else if(events[i].events & EPOLLOUT){// 可写事件
                function<void()> f = std::bind(dealsend, (int)events[i].data.fd, ep);
                tp -> AddTask(f);
            }
    }    
    return ;
}

Acceptor::Acceptor(){
    ini     = new LoadIni();                    // 获取配置文件
    serv    = new Socket();                     // socket
    ep      = new Epoll();                      // IO复用
    tp      = new ThreadPool(ini->core());      // 线程池

    serv->setnonblocking();                     // 设置为非阻塞IO
    ep->addFd(serv->fd(), EPOLLIN | EPOLLET);   // 将此socket 添加到epoll
    serv->online();                             // 服务器上线

    function<void()> f = std::bind(loop, serv, ep, tp);
    tp -> AddTask(f);
    while(1)sleep(100000000);
}
Acceptor::~Acceptor(){
    delete ini;
    delete serv;
    delete ep;
    delete tp;
}
