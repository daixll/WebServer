#include "Connection.h"

std::string fenxi(char* buf){
    std::string res;
    int i=0;    // 解析第一行
    while(buf[i]!=' ') i++; i++;        
    while(buf[i]!=' ') res += buf[i++]; i++;
    

    if(res == "/") res = "/home/jiao/Desktop/doc/html/index.html";
    
    return res;
}

/*
void dealsend(int fd, std::string filename){
    std::string res;
    std::streamsize size = readfile(filename, res);
    // 发送 HTTP 头部信息
    std::string response="HTTP/1.1 200 OK\r\nContent-Length: ";
    response+=i2s(size); response+="\r\n\r\n";
    send(fd, response.c_str(), response.size(), 0);

    long long buffer=0;
    std::streamsize bytes_to_send=4096;
    while (size>0){
        int SendSize = send(fd, res.substr(buffer, std::min(size, bytes_to_send)).c_str(), size, 0);
        
        for(int i=1; i<=1e6+10; i++);

        size = size - SendSize;//用于循环发送且退出功能
        buffer+=SendSize;//用于计算已发buffer的偏移量
    }

    close(fd);
}
*/

/*
void dealsend(int fd, std::string filename){
    // 分段读取文件
    std::ifstream ifs(filename, std::ios::binary);
    war(ifs.is_open(), "文件["+filename+"]打开失败");

    /*
    long long file_size=-1;
    ifs.seekg(0, std::ios::end);
    filesize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    war(file_size==-1, "文件["+filename+"]大小获取错误");
    //

    // 读取文件内容并发送到服务器
    int BUFFER_SIZE=4096;
    std::string buffer(BUFFER_SIZE, '\0');
    while (!ifs.eof()) {
        ifs.read(&buffer[0], BUFFER_SIZE);
        int bytes_read = ifs.gcount();
        send(fd, buffer.c_str(), bytes_read, 0);
        //usleep(100000);
    }

    ifs.close();
}
*/
/*

void dealsend(int fd, Epoll* ep){
    
    std::string filename = ep->fdfile[fd];

    std::string res;
    std::streamsize file_size = readfile(filename, res);
    
    std::cout<<file_size<<"| ";

    // 发送 HTTP 头部信息
    std::string response="HTTP/1.1 200 OK\r\nContent-Length: ";
    response+=i2s(file_size); response+="\r\n\r\n";
    
    send(fd, response.c_str(), response.size(), 0);

    // 分段传输文件
    std::streamsize bytes_sent = 0;
    //while (bytes_sent < file_size) {
    for(long long i=0; bytes_sent < file_size;i++){
        // 读取数据块
        std::streamsize bytes_to_send = std::min((std::streamsize)4096, file_size - bytes_sent);

        //std::cout<<i*4096+bytes_to_send<<" ";

        std::streamsize bytes_sent_now = send(fd, res.substr(i*4096, bytes_to_send).c_str(), bytes_to_send, 0);
        
        // 暂停 0.001 秒钟
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(1)) {
            // 空循环
        }

        bytes_sent += bytes_sent_now;
    }

    close(fd);
}


void dealrecv(int fd, Epoll* ep){
    char buf[10000];
    memset(buf, '\0', sizeof buf);
    while(1){
        ssize_t size = recv(fd, buf, 1024, 0);
        if(size==0){
            printf("!!%d未发送任何消息!!", fd); 
            break ;
        }
        else if(size==-1 && errno == EINTR){
            printf("!!%d客户端中断!!", fd);     
            continue;
        }
        else if(size==-1){ // 现在没有数据可读了, 等会再试
            printf("ok |");     
            break ;
        }
        else{
            printf("%d请求包大小:%ld ", fd, size);
            continue; 
        }
    }

    ep->fdfile[fd]=fenxi(buf);
    ep->modFd(fd, EPOLLOUT | EPOLLET);
    std::cout<<ep->fdfile[fd]<<"| ";
}
*/