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
    std::cout<<YELLOW<<msg<<RESET<<std::endl;
    return flg;
}

long long readfile(const std::string filename, std::string& res, int re){
    // 用二进制打开文件
    std::ifstream ifs(filename, std::ios::binary);  
    
    // 会尝试打开文件re次
    int cnt=re;         
    while(true){
        //err(!cnt--, "文件["+filename+"]打开失败", 0);
        if(ifs.is_open()) break;
        ifs.clear();
        ifs.open(filename);
        if(--cnt<-1) break;
    }
    
    // 会尝试获取文件大小re次
    long long filesize=-1;
    cnt=re;
    while(true){
        //err(!cnt--, "文件["+filename+"]大小-1", 0);
        ifs.seekg(0, std::ios::end);
        filesize = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        if(filesize!=-1) break;
        if(--cnt<-1) break;
    }

    // 获取文件内容
    std::stringstream ss;
    ss<<ifs.rdbuf();
    res = ss.str();

    // 关闭文件
    ifs.close();
    return filesize;
}

LoadIni::LoadIni(std::string filename){
    std::string ini;
    readfile(filename, ini);

    int i=0;
    while(ini[i]!='#') _ip+=ini[i++];                   while(ini[i++]!='\n');
    while(ini[i]!='#') _port=_port*10+ini[i++]-'0';     while(ini[i++]!='\n');
    while(ini[i]!='#') _block=_block*10+ini[i++]-'0';   while(ini[i++]!='\n');
    while(ini[i]!='#') _core=_core*10+ini[i++]-'0';  
}
LoadIni::~LoadIni(){}

std::string LoadIni::ip(){
    return _ip;
}
int LoadIni::port(){
    return _port;
}
long long LoadIni::block(){
    return _block;
}
int LoadIni::core(){
    return _core;
}
