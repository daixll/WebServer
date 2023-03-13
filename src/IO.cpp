#include "IO.h"

void dealrecv(int fd, Epoll* ep, MemoryPool* mp){
    Block* buf = mp->nb(4000);

    while(1){
        ssize_t size = recv(fd, buf->data, buf->size, 0);
        if(size==0){
            printf("!!%d未发送任何消息!!", fd); break ;
        }
        else if(size==-1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){ // 现在没有数据可读了, 等会再试
            printf("%d完成一次读取  ", fd);     break ;
        }
        else if(size==-1 && errno == EINTR){
            printf("!!%d客户端中断!!", fd);     continue;
        }
        else{
            printf("%d请求成功  大小:%ld  %s", fd, size, buf->data);
            
            

            continue;
        }
    }

    printf("%d修改为可写事件  ", fd);
    ep->modFd(fd, EPOLLOUT | EPOLLET);

    mp->sb(buf);
    return ;
}

void dealsend(int fd, Epoll* ep, MemoryPool* mp){
    char response[10240]="HTTP/1.1 200 OK\r\n\r\n";

    FILE* fp=NULL;
    fp=fopen("./html/one.html", "r");

    char text[1024];
    memset(text, '\0', sizeof text);
    fread(text, sizeof(char),1000, fp);strcat(response,text);
    
    send(fd, response,strlen(response), 0);
    printf("%d发送成功!  \n", fd);

    close(fd);

    return ;
}