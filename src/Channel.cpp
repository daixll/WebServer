#include "Channel.h"

Channel::Channel(int fd, Epoll* ep, MemoryPool* mp){
    _fd=fd; _ep=ep; _mp=mp;
}
Channel::~Channel(){}

void Channel::needsend(std::string& file){
    buf = _mp->nb(file.size());
    strcpy(buf->data, file.c_str());
    _ep->modFd(_fd, EPOLLOUT | EPOLLET);
}

char* Channel::sendfile(int size){
    char* res;
    strncpy(res, (buf->data)+last, size);
    last+=size;
    return res;
}