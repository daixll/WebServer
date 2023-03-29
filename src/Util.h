#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// 发生错误
void err(bool flg, std::string msg);
// 警告信息
bool war(bool flg, std::string msg);

// i2s
std::string i2s(long long num);

// 读取filename文件, 内容存在res中, 返回读取的大小
long long readfile(const std::string filename, std::string& res);

// 获取服务器配置信息
class LoadIni
{
public:
    LoadIni(std::string filename="serv.ini");  // 加载初始化文件
    ~LoadIni();

private:
    
};