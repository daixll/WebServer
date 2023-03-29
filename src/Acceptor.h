#pragma once
#include "Util.h"
#include "Socket.h"
#include "Epoll.h"
#include "Channel.h"
#include "ThreadPool.h"
#include "MemoryPool.h"
#include <vector>

class Channel;
class Acceptor{
public:
    Acceptor();
    ~Acceptor();

    std::vector<Channel*> poll(Channel* servch, int timeout);
};