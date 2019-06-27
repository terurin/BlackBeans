#include "shell.h"
#include <string.h>
#include <assert.h>
#include "text.h"
#include<FreeRTOS.h>
#include <stdio.h>

#define BUFFER_SIZE (64)

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
        items[i].context=NULL;
    }
    shell->items=items;
    shell->size=size;
    shell->used=0;
}

bool shell_join(shell_t* shell,const char* name,shell_command_t command,void *context){
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
    insert->context=context;
    return true;
}

bool shell_join_simple(shell_t* shell,const char* name,shell_command_simple_t command){
    return shell_join(shell,name,(shell_command_t)command,NULL);
}

shell_item_t* shell_find(shell_t* shell,const char* name){
    assert(shell);
    const size_t size=shell->size;
    const size_t start=shell_hash(shell,name);
    shell_item_t* const items=shell->items;
    size_t index;
    
    for (index=start;index!=start-1;index=loop_next(index,size)){
        if (!(items[index].command))return NULL;
        if (!strncmp(items[index].name,name,SHELL_NAME_LENGTH)){
            return &items[index];
        }
    }
    return NULL;
}

void shell_parse(shell_t* shell,shell_puts_t writer,char* line){
    assert(shell);
    assert(writer);
    const static int arg_max=16;
    const static char split_tokens[]=" ";
    const char *argv[arg_max];
    size_t argc=text_split(argv,arg_max,line,split_tokens);
    if (!argc){
        writer("\n");
        return;
    }
    shell_item_t* item=shell_find(shell,argv[0]);
    if (!item){
        writer("Error:Not Found\n");
        return ;
    }
    item->command(writer,argc,argv,item->context);
}

void shell_join_basic(shell_t* shell){
    assert(shell);
    shell_join(shell,"dump",(shell_command_t)command_dump,shell);
    shell_join_simple(shell,"echo",command_echo);
    shell_join_simple(shell,"rtos",command_rtos);
    
    
}

void command_dump(shell_puts_t writer,int argc,const char** argv,shell_t* shell){
    (void)argc;
    (void)argv;
    assert(shell);
    assert(writer);
    char* buffer = (char*)pvPortMalloc(sizeof(char)*BUFFER_SIZE);
    snprintf(buffer,BUFFER_SIZE,"shell [%d/%d]\n",shell->used,shell->size);
    writer(buffer);
    //要素吐き出し
    const shell_item_t* const items=shell->items;
    const size_t size= shell->size; 
    for (size_t i=0;i<size;i++){
        const char *name=items[i].name;
        snprintf(buffer,BUFFER_SIZE,"%d:%d->%s\n",i,shell_hash(shell,name),name);
        writer(buffer);
    }
    vPortFree(buffer);
}

void command_echo(shell_puts_t writer,int argc,const char** argv){
    assert(writer);
    if (argc<2)return;
    char* buffer = (char*)pvPortMalloc(sizeof(char)*BUFFER_SIZE);
    for (int i=1;i<argc-1;++i){ 
        sniprintf(buffer,BUFFER_SIZE,"%s ",argv[i]);
        writer(buffer);
    }
    sniprintf(buffer,BUFFER_SIZE,"%s\n",argv[argc-1]);
    writer(buffer);    
    vPortFree(buffer);
}

void command_rtos(shell_puts_t writer,int argc,const char** argv){
    //
    char* buffer = (char*)pvPortMalloc(sizeof(char)*BUFFER_SIZE);
    size_t heap_total=configTOTAL_HEAP_SIZE;
    size_t heap_size=xPortGetFreeHeapSize();
    size_t heap_linear=xPortGetMinimumEverFreeHeapSize();
    sniprintf(buffer,BUFFER_SIZE,
        "total:%d,used:%d,linear:%d\n",
        heap_total,heap_total-heap_size,heap_linear);
    writer(buffer);
    vPortFree(buffer);
}