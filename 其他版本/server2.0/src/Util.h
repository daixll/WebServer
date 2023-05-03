#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>

// 发生错误
void err(bool flg, std::string msg);
// 警告信息
bool war(bool flg, std::string msg);
// 读取filename文件, 内容存在res中, 返回读取的大小
long long readfile(const std::string filename, std::string& res, int re=1);

// 获取服务器配置信息
class LoadIni
{
public:
    LoadIni(std::string filename="serv.ini");  // 加载初始化文件
    ~LoadIni();

    std::string ip();
    int         port();
    long long   block();// 内存块最大大小(单个文件最大大小)
    int         core(); // 线程数量
private:
    std::string _ip;
    int         _port=0;
    long long   _block=0;
    int         _core=0;
};