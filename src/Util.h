#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

void errif(bool, const char*);

class LoadIni
{
public:
    LoadIni();  // 加载初始化文件
    ~LoadIni();

    char*       ip();
    int         port();
    long long   block();// 内存块最大大小(单个文件最大大小)
    int         core(); // 线程数量
private:
    char        _ip[128];
    int         _port=0;
    long long   _block=0;
    int         _core=0;
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