#include "Util.h"

void errif(bool condition, const char *errmsg){
    if(condition){
        perror(errmsg); 
        exit(EXIT_FAILURE);
    }
}

LoadIni::LoadIni(){
    char line[138];
    FILE* fp = fopen("serv.ini", "r");
    
    fgets(line, 128, fp);
    for (int i=0; line[i]!='#'; i++) _ip[i]=line[i];
    
    fgets(line, 128, fp);
    for (int i=0; line[i]!='#'; i++) _port=_port*10+line[i]-'0';

    fgets(line, 128, fp);
    for (int i=0; line[i]!='#'; i++) _block=_block*10+line[i]-'0';

    fgets(line, 128, fp);
    for (int i=0; line[i]!='#'; i++) _core=_core*10+line[i]-'0';
    
    fclose(fp);
}
LoadIni::~LoadIni(){}

char* LoadIni::ip(){
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