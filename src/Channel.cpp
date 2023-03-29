#include "Channel.h"

Channel::Channel(int fd, Epoll* ep, MemoryPool* mp){
    _fd=fd; _ep=ep; _mp=mp;
}
Channel::~Channel(){}

// fd
int Channel::fd(){
    return _fd;
}
Epoll* Channel::ep(){
    return _ep;
}
// events
uint32_t Channel::events(){
    return _events;
}
void Channel::setevents(uint32_t ev){
    _events=ev;
}
// inepoll
bool Channel::inepoll(){
    return _inep;
}
void Channel::setinepoll(bool st){
    _inep = st;
}