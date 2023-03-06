#include "Util.h"

void errif(bool condition, const char *errmsg){
    if(condition){
        perror(errmsg); 
        exit(EXIT_FAILURE);
    }
}

LoadIni::LoadIni(){
    port=0;
    char line[138];
    FILE *fp = fopen("set.ini", "r");
    
    fgets(line, 128, fp);
    for (int i=0; line[i]!='#'; i++) ip[i]=line[i];
    
    fgets(line, 128, fp);
    for (int i=0; line[i]!='#'; i++) port=port*10+line[i]-'0';

    fgets(line, 128, fp);
    for (int i=0; line[i]!='#'; i++) blocksize=blocksize*10+line[i]-'0';

    fgets(line, 128, fp);
    for (int i=0; line[i]!='#'; i++) corepoolsize=corepoolsize*10+line[i]-'0';
    
    fclose(fp);
}
LoadIni::~LoadIni(){}

char* LoadIni::getip(){
    return ip;
}
int LoadIni::getport(){
    return port;
}
long long LoadIni::getblocksize(){
    return blocksize;
}
int LoadIni::getcorepoolsize(){
    return corepoolsize;
}