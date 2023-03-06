#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

void errif(bool, const char*);

class LoadIni
{
private:
    char ip[128];
    int port=0;
    long long blocksize=0;
    int corepoolsize=0;

public:
    LoadIni(); // 初始化
    ~LoadIni();
    char* getip();
    int getport();
    long long getblocksize();
    int getcorepoolsize();
};

#endif

/*
#ifndef UTIL_H:
    防止头文件的重复包含及编译
    if not define的简写

#endif
    #ifndef
    ...
    #endif
*/