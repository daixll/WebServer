#include "src/Util.h"
#include "src/MemoryPool.h"
#include "src/Acceptor.h"
#include "src/Epoll.h"
#include "src/ThreadPool.h"
#include "src/Socket.h"

int main(){
    LoadIni*    ini = new LoadIni();
    MemoryPool* mp  = new MemoryPool();
    Epoll*      ep  = new Epoll(mp);
    ThreadPool* tp  = new ThreadPool(3);
    Acceptor*   ac  = new Acceptor();
    // 服务器socket
    Socket*     servsock = new Socket();
    servsock->setnonblocking();
    // 服务器Channel
    Channel*    servch = new Channel(servsock->fd, ep, mp);
    servch->setevents(EPOLLET | EPOLLIN);
    // 服务器上线 开始监听
    servsock -> online();

    //Acceptor* ac = new Acceptor(servch, -1);
    while(true){
        // 处理连接
        std::vector<Channel*> events = ac->poll(servch, -1);
        
        // 发送给 SubReactor Connection
        for(auto ch: events){
            function<void()> f = std::bind(dealrecv, (int)events[i]->ev.data.fd, ep);
            tp -> AddTask(f);
        }        
    }

    return 0;
}

/*
int main(){
    Socket*     serv    = new Socket();                     // socket
    Epoll*      ep      = new Epoll();                      // IO复用
    ThreadPool* tp      = new ThreadPool(3);      // 线程池
    MemoryPool* mp      = new MemoryPool((long long)2*1024*1024*1024);

    serv->setnonblocking();                     // 设置为非阻塞IO
    ep->addFd(serv->fd, EPOLLIN | EPOLLET);   // 将此socket 添加到epoll
    serv->online();                             // 服务器上线

    while(true){
        std::vector<Channel*> events = ep->poll(-1, mp);    // 就绪事件
        
        for(int i=0; i<events.size(); i++)
            if(events[i]->ev.data.fd == serv->fd){  // 新的事件
                
                int clnt = socket(AF_INET6, SOCK_STREAM, 0);    // 创建套接字
                sockaddr_in6 addr; int addr_len = sizeof addr;  // ip相关
                clnt = serv->ac6(addr, addr_len);
                fcntl(clnt, F_SETFL, fcntl(clnt, F_GETFL) | O_NONBLOCK);

                printf("\n新的连接%d |", clnt);

                Channel* c = new Channel(clnt, ep, mp);
                ep->addFd(clnt, EPOLLIN | EPOLLET);
            }
            else if(events[i]->ev.events & EPOLLIN){// 可读事件
                //function<void()> f = std::bind(dealrecv, (int)events[i]->ev.data.fd, ep);
                //tp -> AddTask(f);
            }
            else if(events[i]->ev.events & EPOLLOUT){// 可写事件
                //function<void()> f = std::bind(dealsend, (int)events[i]->ev.data.fd, ep);
                //tp -> AddTask(f);
            }
    }    

    return 0;
}
*/