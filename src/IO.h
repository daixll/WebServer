#include "Socket.h"
#include "Epoll.h"
#include "MemoryPool.h"

void dealrecv(int fd, Epoll* ep, MemoryPool* mp);

void dealsend(int fd, Epoll* ep, MemoryPool* mp);