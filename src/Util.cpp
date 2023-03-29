#include "Util.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"

void err(bool flg, std::string msg){
    if(!flg) return ;
    std::cerr<<RED<<msg<<RESET<<std::endl;
    exit(0);
}

bool war(bool flg, std::string msg){
    if(flg) std::cerr<<YELLOW<<msg<<RESET<<std::endl;
    return flg;
}

std::string i2s(long long num){
    std::stringstream ss;
    ss<<num;
    std::string res;
    ss>>res;
    return res;
}  

long long readfile(const std::string filename, std::string& res){
    // 用二进制打开文件
    std::ifstream ifs(filename, std::ios::binary);  
    if(war(!ifs.is_open(), "文件["+filename+"]打开失败"))
        return -1;
    // 获取文件大小
    long long filesize=-1;
    ifs.seekg(0, std::ios::end);
    filesize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    if(war(filesize==-1, "文件["+filename+"]大小获取失败"))
        return -1;
    // 获取文件内容
    std::stringstream ss;
    ss<<ifs.rdbuf();
    res = ss.str();
    // 关闭文件
    ifs.close();
    return filesize;
}

LoadIni::LoadIni(std::string filename){

}
LoadIni::~LoadIni(){

}