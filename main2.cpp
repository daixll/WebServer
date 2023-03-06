#include "src/Util.h"
#include "src/Socket.h"
#include "src/MemoryPool.h"
#include "src/Epoll.h"
#include "src/ThreadPool.h"

void handleReadEvents(int sockfd){
    char buf[10240];
    while(true){
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        
        if(bytes_read > 0){
            printf("信息来自 fd: %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        }   // 客户端正常中断
        else if(bytes_read == -1 && errno == EINTR){
            printf("继续");
            continue;
        }   // 非阻塞IO,表示数据全部读取完
        else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
            printf("完成一次读入, errno: %d\n", errno);
            break;
        }   // EOF 客户端断开连接
        else if(bytes_read == 0){
            printf("EOF, fd %d 断开连接\n", sockfd);
            close(sockfd);  // 关闭socket会自动将文件描述符从epoll上移除
            break;
        }
    }
}

int main(){
    // 获取服务器配置信息
    LoadIni* serv_set = new LoadIni();
    
    // 创建服务器
    InetAddress *serv_ip = new InetAddress(serv_set->getip(), serv_set->getport());
    Socket *serv_sock = new Socket();
    serv_sock->bind(serv_ip);
    serv_sock->listen();
    serv_sock->setnonblocking();
    
    // 创建epoll
    Epoll *ep = new Epoll();
    ep->addFd(serv_sock->getFd(), EPOLLIN | EPOLLET);

    while(true){
        std::vector<epoll_event> events = ep->poll();
        int nfds = events.size();
        for(int i=0; i<nfds; i++)
            if(events[i].data.fd == serv_sock->getFd()){    // 新客户端连接
                InetAddress *clnt_addr  = new InetAddress("[::]", 0);
                Socket *clnt_sock       = new Socket();
                clnt_sock->bind(clnt_addr);
                
                printf("新的连接\n");
                
                clnt_sock->setnonblocking();
                ep->addFd(clnt_sock->getFd(), EPOLLIN | EPOLLET);
                    //delete clnt_addr;
                    //delete clnt_sock;
            }   // 可读事件
            else if(events[i].events & EPOLLIN){
                handleReadEvents(events[i].data.fd);
                
            }
            else{

                printf("其他事件!\n");      
            }
    }
    
    delete serv_sock;
    
    return 0;
}

