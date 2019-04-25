#include "shell.h"
#include <string.h>

char* shell_parse(char* line,size_t size){
    const static int arg_max=16;
    const static char split[]=" ";
    int args;
    const char *argv[arg_max];
    //単語ごとに切り分け
    int argc;
    for (argc=0;argc<arg_max;argc++){
        char *it=strtok(line,split);
        if (it==NULL)break;
        argv[argc]=it;
    }
    

}