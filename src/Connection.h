#pragma once
#include "Util.h"
#include "Socket.h"
#include "Epoll.h"
#include "Channel.h"

void conn(Channel* ch);

void dealrecv(Channel* ch);
void dealsend(Channel* ch);