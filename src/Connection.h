#pragma once
#include "Util.h"
#include "Socket.h"
#include "Epoll.h"
#include "chrono"

void dealrecv(int fd, Epoll* ep);
void dealsend(int fd, Epoll* ep);