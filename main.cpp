#include <cstring>
#include <string>
#include <iostream>
#include <sstream>

#include "src/Util.h"
#include "src/Socket.h"
#include "src/MemoryPool.h"
#include "src/Epoll.h"
#include "src/ThreadPool.h"
using namespace std;

#define MAX_EVENTS 1024

#define debug 1


int s2i(string s){
    stringstream ss;
    ss<<s;
    int num;
    ss>>num;
    return num;
}

struct rq_line{
    string method;  // 请求方法
    string url;     // 请求链接
    string httpV;   // http版本
};
struct rq_head{
    unordered_map<string, string> H;
};

/* 功能函数 */
void do_http_request(int sock); // 处理HTTP请求
    rq_line request_line(int sock);             // 请求行
    rq_head request_header(int sock);           // 请求报头

/* 根据请求方法的各种返回 */
void method_get(int sock, string url, int len);
void method_post(string url);

// ********************************************************

rq_line request_line(int sock){
    rq_line line;   // 请求行数据
    char c='*';
    while(read(sock, &c, 1)==1)
        if(c==' ') break;
        else line.method+=c;
    while(read(sock, &c, 1)==1)
        if(c==' ') break;
        else line.url+=c;
    while(read(sock, &c, 1)==1)
        if(c=='\n') break;
        else line.httpV+=c;
    return line;
}

rq_head request_header(int sock){
    rq_head hand;
    string key="", value="";
    char c;

    // 接收整个hand
    while(true){
        key=value="";
        bool flg=0; // 接收key 
        // 接收一行
        while(true){
            read(sock, &c, 1);
            if(c=='\n') break;
            
            if(c==':'){ // 后面开始接收value
                flg=1;
                continue;
            }
            
            if(!flg)
                key+=c;
            else
                value+=c;
        }

        if(key=="\n" || key=="\r" || key.size()<=1) break;
        hand.H[key]=value;
    }

    return hand;
}

string request_body(int sock){
    string res;
    char c[100];
    //memset(c, '0', sizeof c);
    read(sock, c, sizeof c);
    for(int i=0; c[i]!='\n'; i++) res+=c[i];
    return res+"\0";
}

void method_get(int sock, string url){
    // 返回的内容
    char response[10240]="HTTP/1.1 200 OK\r\n\r\n"; 
    
    FILE *fp=NULL;
    fp=fopen("./html/index.html", "r");
    char text[10024];
    memset(text, '\0', sizeof text);
    fread(text, sizeof(char), 10000, fp);
    
    strcat(response, text);

    //for(int i=0; i<800; i++) cout<<response[i];

    write(sock, response, strlen(response));
    return ;
}

void method_post(int sock, string url){
    // 返回的内容
    char response[10240]="HTTP/1.1 202 OK\r\n\r\n"; 

    FILE *fp=NULL;
    char URL[200]="./html";
    for(int i=0; url[i]; i++) URL[i+6]=url[i];

    fp=fopen(URL, "r");
    char text[10024];
    memset(text, '\0', sizeof text);
    fread(text, sizeof(char), 10000, fp);
    
    strcat(response, text);

    //for(int i=0; i<800; i++) cout<<response[i];
    //cout<<sock<<" ";
    write(sock, response, strlen(response));
    //cout<<sock<<" ";
    //for(int i=0; i<200; i++) cout<<response[i];

    return ;
}

void do_http_request(int sock){

    /*
    for(int i=1; i<=1e2; i++){
        int a[500010];
        for(int j=1; j<=500000; j++)
            a[j]=i;
    }
    */

    rq_line line = request_line(sock);
    rq_head head = request_header(sock);
    

    if(line.method=="GET"){
        //function<void()> f = std::bind(method_get, sock, line.url);
        //Poll -> AddTask(f);
        
        method_get(sock, line.url);
    }
    if(line.method=="POST"){
        //cout<<sock<<" ";
        string body = request_body(sock);
        // 解析密码！
        
        string a="", b="";
        int i=0;
        for( ; body[i]!='='; i++);
        for(i++; body[i]!='&'; i++) a+=body[i];
        a+="\0";
        for( ; body[i]!='='; i++);
        for(i++; body[i]!='\0'; i++) b+=body[i];
        b+="\0";

        cout<<"["<<a<<"] ["<<b<<"]\n";

        cout<<(b=="jiao")<<"\n";

        if(a=="root" && b=="jiao"){
            cout<<"666";
            method_post(sock, line.url);
        }
    }


    return ;
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




    int epfd = epoll_create1(0);
    errif( epfd == -1, "epoll 创建错误!"); 

    struct epoll_event events[MAX_EVENTS], ev;
    bzero(&events, sizeof(events));
    bzero(&ev, sizeof(ev));

    ev.data.fd = serv_sock->getFd();			// 该IO口为服务器socket fd
	ev.events = EPOLLIN | EPOLLPRI | EPOLLOUT | EPOLLET;	
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock->getFd(), &ev);// 添加事件到epoll





    ThreadPoll* Poll = new ThreadPoll(serv_set->getcorepoolsize());

    while (true){   // 不断监听epoll上的事件
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1); // nfds个事件
        for(int i=0; i<nfds; i++)
            if(events[i].data.fd == serv_sock->getFd()){    // 新客户端连接
                InetAddress *clnt_addr  = new InetAddress("[::]", 0);
                Socket *clnt_sock       = new Socket();
                clnt_sock->bind(clnt_addr);
                
                printf("新的连接%d\n", i);
                
                bzero(&ev, sizeof(ev));
				ev.data.fd = clnt_sock->getFd();
				ev.events = EPOLLIN | EPOLLPRI | EPOLLOUT | EPOLLET;	// 客户端连接 ET模式
				clnt_sock->setnonblocking();	// 非阻塞式socket
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock->getFd(), &ev);
            }   // 可读事件
            else if(events[i].events & EPOLLIN){
                //function<void()> f = std::bind(do_http_request, events[i].data.fd);
                //Poll -> AddTask(f);
                printf("发送数据\n");
                do_http_request(events[i].data.fd);
                
                close(events[i].data.fd);	// 关闭socket会自动将文件描述符从epoll数上移除
            }
    }
    delete serv_sock;
    delete Poll;
    return 0;
}