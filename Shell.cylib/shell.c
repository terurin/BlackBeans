#include "shell.h"
#include <string.h>
#include <assert.h>
#include "text.h"
#include<FreeRTOS.h>
#include <stdio.h>

static inline size_t shell_hash(const shell_t* shell,const char* str){
    const size_t rem =shell->size;
    size_t hash=0;
    for (const char* it=str;*it!=0;it++){
        hash=(hash<<1)^*it;
    }
    return hash%rem;
}

static inline size_t loop_next(size_t idx,size_t size){
    const size_t next =idx+1;
    return next<size?next:0;
}

void shell_init(shell_t* shell,size_t size){
    assert(shell);
    shell_item_t *items=(shell_item_t*)pvPortMalloc(size* sizeof(shell_item_t));
    for (size_t i=0;i<size;i++){
        memset(items[i].name,0,SHELL_NAME_LENGTH*sizeof(char));
        items[i].command=NULL;
    }
    shell->items=items;
    shell->size=size;
    shell->used=0;
}

bool shell_join(shell_t* shell,const char* name,shell_command_t command){
    assert(shell);
    assert(name);
    assert(command);
    //空いていいる場所を探す
    const size_t size=shell->size;
    const size_t start=shell_hash(shell,name);
    shell_item_t* const items=shell->items;
    size_t index;
    for (index=start;(items[index].command)!=NULL;index=loop_next(index,size)){
        if (index==start-1)return false;//空きがない
    }
    //挿入
    shell_item_t* insert=&shell->items[index];
    strncpy(insert->name,name,SHELL_NAME_LENGTH-1);
    insert->command=command;
    return true;
}

shell_command_t shell_find(shell_t* shell,const char* name){
    assert(shell);
    const size_t size=shell->size;
    const size_t start=shell_hash(shell,name);
    const shell_item_t* const items=shell->items;
    size_t index;
    
    for (index=start;index!=start-1;index=loop_next(index,size)){
        if (!(items[index].command))return NULL;
        if (!strncmp(items[index].name,name,SHELL_NAME_LENGTH)){
            return items[index].command;
        }
    }
    return NULL;
}

char* shell_parse(shell_t* shell,char* buffer,size_t buffer_size,char* line){
    assert(shell);
    const static int arg_max=16;
    const static char split_tokens[]=" ";
    const char *argv[arg_max];
    size_t argc=text_split(argv,arg_max,line,split_tokens);
    if (!argc)return "\r";
    shell_command_t command=shell_find(shell,argv[0]);
    if (!command)return "Error:Not Found\r";
    
    buffer[0]=0;
    return command(buffer,buffer_size,argv,argc);
}

void shell_dump(const shell_t* shell,shell_puts_t writer){
    assert(shell);
    assert(writer);
    #define BUFFER_SIZE (64)
    static char buffer[BUFFER_SIZE];
    snprintf(buffer,BUFFER_SIZE,"shell [%d/%d]\n",shell->used,shell->size);
    writer(buffer);
    //要素吐き出し
    const shell_item_t* const items=shell->items;
    const size_t size= shell->size; 
    for (size_t i=0;i<size;i++){
        const char *name=items[i].name;
        snprintf(buffer,BUFFER_SIZE,"%d:%d->%s\n",i,shell_hash(&shell,name),name);
        writer(buffer);
    }
}