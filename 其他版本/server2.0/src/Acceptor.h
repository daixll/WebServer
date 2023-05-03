#pragma once
#include "Util.h"
#include "Socket.h"
#include "Epoll.h"
#include "ThreadPool.h"
#include "Connection.h"

// 事件循环, 其实就是封装一下ep
void loop(Socket* serv, Epoll* ep, ThreadPool* tp);

class Acceptor{
public:
    Acceptor();     // 主Reactor
    ~Acceptor();
private:
    LoadIni*    ini;
    Socket*     serv;
    Epoll*      ep;
    ThreadPool* tp;
};