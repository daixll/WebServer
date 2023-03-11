#include "src/Socket.h"
#include "src/MemoryPool.h"
#include "src/Epoll.h"
#include "src/ThreadPool.h"
#include "src/IO.h"

int main(){
    LoadIni*    ini     = new LoadIni();                    // 获取配置文件
    Sock*       serv    = new Sock(ini->ip(), ini->port()); // socket
    Epoll*      ep      = new Epoll();                      // IO复用
    ThreadPool* tp      = new ThreadPool(ini->core());      // 线程池
    MemoryPool* mp      = new MemoryPool(ini->block());     // 内存池
    
    serv->setnonblocking();                 // 服务器 socket 改为非阻塞
    ep->addFd(serv->fd, EPOLLIN | EPOLLET); // 将此socket 添加到epoll
    serv->online();                         // 服务器上线

    while(1){
        std::vector<epoll_event> events = ep->poll(-1);    // 就绪事件
        for(int i=0; i<events.size(); i++)
            if(events[i].data.fd == serv->fd){  // 新的事件
                Sock* clnt = new Sock("", 0);
                clnt->fd = serv->start(clnt->addr(), clnt->addr_len());
                clnt->setnonblocking();

                printf("新的连接: %d\n", clnt->fd);

                ep->addFd(clnt->fd, EPOLLIN | EPOLLET);
            }
            else if(events[i].events & EPOLLIN){// 可读事件
                dealrecv(events[i].data.fd);
                ep->modFd(events[i].data.fd, EPOLLOUT | EPOLLET);
            }
            else if(events[i].events & EPOLLOUT){// 可写事件
                dealsend(events[i].data.fd);
                close(events[i].data.fd);
            }
    }    

    delete ini;
    delete serv;
    delete ep;
    delete tp;
    delete mp;
    return 0;
}