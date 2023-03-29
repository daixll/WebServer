#include "Acceptor.h"

Acceptor::Acceptor(){}
Acceptor::~Acceptor(){}

std::vector<Channel*> Acceptor::poll(Channel* servch, int timeout){
    return servch->ep()->poll(timeout);
}