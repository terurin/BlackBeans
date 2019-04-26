#pragma once
#ifndef __SHELL_HEADER_GUARD__
#define __SHELL_HEADER_GUARD__
    #include <stddef.h>
    #include <stdbool.h>
    
    typedef char* (*shell_command_t)(char* buffer,size_t buffer_size,const char** argv,int argc);
    #define SHELL_NAME_LENGTH (8)
    typedef struct shell_item{
        char name[SHELL_NAME_LENGTH];
        shell_command_t command;
    }shell_item_t;
    
    typedef struct shell{
        shell_item_t *items;
        size_t size;
        size_t used;
    }shell_t;

    void shell_init(shell_t* shell,size_t item_size);//item_sizeは実際に入れるitemの2倍ほどにしておくこと
    bool shell_join(shell_t* shell,const char* name,shell_command_t);
    shell_command_t shell_find(shell_t* shell,const char*);
    char* shell_parse(shell_t*,char* buffer,size_t buffer_size,char* line);
    
#endif
    