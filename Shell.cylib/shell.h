#pragma once
#ifndef __SHELL_HEADER_GUARD__
#define __SHELL_HEADER_GUARD__
    #include <stddef.h>
    #include <stdbool.h>
    typedef void (*shell_puts_t)(const char*);
    typedef void (*shell_command_simple_t)(shell_puts_t,int argc,const char** argv);
    typedef void (*shell_command_t)(shell_puts_t,int argc,const char** argv,void* context);
    #define SHELL_NAME_LENGTH (8)
    typedef struct shell_item{
        char name[SHELL_NAME_LENGTH];
        shell_command_t command;
        void* context;
    }shell_item_t;
    
    typedef struct shell{
        shell_item_t *items;
        size_t size;
        size_t used;
    }shell_t;

    void shell_init(shell_t* shell,size_t item_size);//item_sizeは実際に入れるitemの2倍ほどにしておくこと
    bool shell_join(shell_t* shell,const char* name,shell_command_t,void *context);
    bool shell_join_simple(shell_t* shell,const char* name,shell_command_simple_t command);
    
    shell_item_t* shell_find(shell_t* shell,const char*);
    void shell_parse(shell_t*,shell_puts_t,char* line);
    
    void shell_join_basic(shell_t*);
    void command_dump(shell_puts_t writer,int argc,const char** argv,shell_t* shell);
    void command_echo(shell_puts_t writer,int argc,const char** argv);
    void command_rtos(shell_puts_t writer,int argc,const char** argv);
#endif
    