#include "text.h"
#include <string.h>
#include <assert.h>

size_t text_split(const char** argv,size_t argv_size,char *line,const char* token){
    const char* word;
    char* line_pos;
    assert(argv_size);
    
    argv[0]=word=strtok_r(line,token,&line_pos);
    size_t argc=0;
    for (argc=1;argc<argv_size;argc++){
        word=strtok_r(NULL,token,&line_pos);
        if (!word)break;
        argv[argc]=word;
    }
    return argc;
}