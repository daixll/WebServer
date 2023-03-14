#include "IO.h"

void s2cc(std::string s, char* res){
    for(int i=0; i<s.size(); i++)
        res[i]=s[i]; 
    res[s.size()]='\0';
}

void analyse(Block* Buf, ssize_t size, int fd, Epoll* ep){    // 解析数据
    char* buf = Buf->data;

    int a=0;

    int i=0;    // 解析第一行
    while(i<size && buf[i]!=' ') ep->request[fd].method  +=  buf[i++]; i++;        

    while(i<size && buf[i]!=' ') ep->request[fd].file    +=  buf[i++]; i++;

    while(i<size && buf[i]!='\n')ep->request[fd].httpv   +=  buf[i++]; i++;

    while(i < size){
        std::string one, two;
        while(i < size && buf[i]!=':') one+=buf[i++]; i+=2;
        while(i < size && buf[i]!='\n')two+=buf[i++]; i++;
        ep->request[fd].head[one]=two;
        if(i < size && buf[i]=='\n') break;// 说明后面还有
    }   
    
    printf("解析成功!\n");
    return ;
}

void dealrecv(int fd, Epoll* ep, MemoryPool* mp){
    Block* buf = mp->nb(2*1000*1000);

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
            printf("%d请求成功  大小:%ld\n%s", fd, size, buf->data);
            analyse(buf, size, fd, ep);
            continue; 
        }
    }

    printf("%d修改为可写事件  ", fd);
    ep->modFd(fd, EPOLLOUT | EPOLLET);

    mp->sb(buf);
    return ;
}

void dealsend(int fd, Epoll* ep, MemoryPool* mp){
    // 定位文件
    Block* file = mp->nb(2*1000*1000);
    std::string File = ep->request[fd].file;
    if(File=="/") File="/index.html";
    File = "./html"+File;
    
    s2cc(File, file->data);
    
    printf("定位文件:(%s)", file->data);

    // 打开文件
    FILE* fp=NULL;
    int mod=1;  // 需要一个分析返回文件的类型
    switch(mod){
        case 1: fp=fopen(file->data, "r");  break;
        case 2: fp=fopen(file->data, "wr"); break; 
    }
    if(!fp){    // 文件未找到
        char response[64]="HTTP/1.1 404 not found\r\n\r\n页面不存在!";
        send(fd, response, 64, 0);
    }
    else{
        printf("找到文件!  ");

        // 找到了 准备发送
        long long len = fread(file->data, sizeof(char), file->size, fp);
        Block* response = mp->nb(len);
        char OK[32] = "HTTP/1.1 200 OK\r\n\r\n";
        //response->data = "HTTP/1.1 200 OK\r\n\r\n";
        for(long long i=0;  i<19;   i++) response->data[i]=OK[i];
        for(long long i=19; i<len+19;  i++) response->data[i]=file->data[i-19];
        response->data[19+len]='\0';
        send(fd, response->data, len+19, 0);

        printf("\n(%lld)  \n%s", len+20, response->data);

        //printf("\n\n%s\n\n 发送成功!(%lld)  \n", response->data, len);
        //mp->sb(response);
    }
    close(fd);
    mp->sb(file);
    return ;
}

/*
void dealsend(int fd, Epoll* ep, MemoryPool* mp){
    Block* response = mp->nb(1000*1000);
    memset(response->data, '\0', response->size);

    response->data = "HTTP/1.1 200 OK\r\n\r\n\0";

    std::string s = ep->file[fd];
    std::cout<<"目标文件：["<<s<<"] ";


    FILE* fp=NULL;

    // 定位文件
    if(s=="/") s="./html/index.html";
    else s = "./html"+s;
    Block* target = mp->nb(128);
    s2cc(s, target->data);

    //std::cin>>s;

    fp=fopen(target->data, "r");

    Block* text = mp->nb(300*1000); 
    memset(text->data, '\0', text->size);
    
    
    //fread(text->data, sizeof(char), strlen(text->data), fp); 
    
    //for(long long i=0; text->data[i]!='\0'; i++)
    //    response->data[20+i]=text->data[i];
    
    //send(fd, response->data, strlen(text->data), 0);
    printf("%d发送成功!  \n", fd);

    mp->sb(response);
    mp->sb(text);
    mp->sb(target);

    close(fd);

    return ;
}
*/

