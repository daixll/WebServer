#include "IO.h"

void dealrecv(int fd){
    char buf[10240];
    while(1){
        bzero(&buf, strlen(buf));

        ssize_t size = recv(fd, buf, 1024, 0); ///???
        if(size==0){
            printf("%d未发送任何消息\n", fd);
            return ;
        }
        else if(size==-1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){ // 现在没有数据可读了, 等会再试
            printf("%d完成一次读取\n", fd);
            return ;
        }
        else if(size==-1 && errno == EINTR){
            printf("%d客户端中断\n", fd);
            continue;
        }
        else{
            printf("%d请求消息:\n%s", fd, buf);
            // deal消息
            continue;
        }
    }

    return ;
}

void dealsend(int fd){
    char response[10240]="HTTP/1.1 200 OK\r\n\r\n";

    FILE* fp=NULL;
    fp=fopen("./html/index.html", "r");

    char text[ 1024];
    memset(text, '\0', sizeof text);
    fread(text, sizeof(char),1000, fp);strcat(response,text);
    printf("发送内容:\n%s\n", response);
    
    
    send(fd, response,strlen(response), 0);

    return ;
}