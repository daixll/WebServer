#include "src/Socket.h"
#include "src/MemoryPool.h"
#include "src/Epoll.h"
#include "src/ThreadPool.h"

MemoryPool* mp;
/*
void sendyw2(int fd){
    printf("发送%d\n", fd);

    // 文件 业务
    Block* res = mp->nb(500*1024*1024);
    Block* one = mp->nb(500*1024*1024);

    mp->w2b("html/head.txt", one->data);
    mp->w2b("html/index.html", res->data);

    strcat(one, res);

    // 发送
    send(fd, res->data, strlen(res->data), 0);
    close(fd);
}
*/


void sendyw(int fd){
    // 返回的内容
    char response[1024]="HTTP/1.1 200 OK\r\n\r\n"; 

    FILE *fp=NULL;
    //char URL[200]="./html";
    //for(int i=0; url[i]; i++) URL[i+6]=url[i];

    printf("正常读取\n");

    fp=fopen("html/index.html", "r");
    char text[1024];
    memset(text, '\0', sizeof text);
    fread(text, sizeof(char), 1000, fp);
    
    

    strcat(response, text);
    send(fd, response, strlen(response), 0);
    close(fd);
}


int main(){
    LoadIni* ini = new LoadIni();
    Sock* serv = new Sock(ini->ip(), ini->port());  // 初始化
    mp = new MemoryPool(ini->block());
    ThreadPool* tp = new ThreadPool(ini->core()); 

    // epool
    Epoll* ep = new Epoll();
    serv->setnonblocking();
    ep->addFd(serv->fd, EPOLLIN | EPOLLET);

    serv->online();                     // 服务器上线
    while(1){
        std::vector<epoll_event> events = ep->poll(-1);    // 事件些
        for(int i=0; i<events.size(); i++){
            
            if(events[i].data.fd == serv->fd){
                // 客户端 socket
                Sock *clnt = new Sock("", 0);
                clnt->fd = serv->start(clnt->addr(), clnt->addr_len());
                clnt->setnonblocking();

                printf("新的连接: %d\n", clnt->fd);

                ep->addFd(clnt->fd, EPOLLIN | EPOLLET);
            }
            else if(events[i].events & EPOLLIN){
                printf("读取\n");

                /*
                char buf[1024];
                while(1){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
                    bzero(&buf, sizeof(buf));
                    ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
                    if(bytes_read > 0){
                        printf("message from client fd %d: %s\n", events[i].data.fd, buf);
                        //write(events[i].data.fd, buf, sizeof(buf));
                    } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
                        printf("continue reading");
                        continue;
                    } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
                        printf("finish reading once, errno: %d\n", errno);
                        //close(events[i].data.fd);   //关闭socket会自动将文件描述符从epoll树上移除
                        break;
                    } else if(bytes_read == 0){  //EOF，客户端断开连接
                        printf("EOF, client fd %d disconnected\n", events[i].data.fd);
                        ep->modFd(events[i].data.fd, EPOLLOUT | EPOLLET);
                        //close(events[i].data.fd);   //关闭socket会自动将文件描述符从epoll树上移除
                        break;
                    }
                }
                */

                ep->modFd(events[i].data.fd, EPOLLOUT | EPOLLET);
            }
            else if(events[i].events & EPOLLOUT){

                printf("开始发送！");

                int fd = events[i].data.fd; 
                function<void()> f = std::bind(sendyw, fd);
                tp -> AddTask(f);
                //close(events[i].data.fd);

                // 文件 业务
                //Block* res = mp->nb(500*1024*1024);
                //mp->w2b("html/index.html", res->data);
                // 发送
                //send(events[i].data.fd, res->data, strlen(res->data), 0);
                //close(events[i].data.fd);

            }
        }
    }    

    delete serv;
    return 0;
}