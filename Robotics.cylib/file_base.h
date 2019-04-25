#pragma once
#ifndef __FILE_BASE_HEADER_GUARD__
#define __FILE_BASE_HEADER_GUARD__
#include <stddef.h>

typedef size_t (*file_write_t)(void*,const void*,size_t);
typedef size_t (*file_read_t)(void*,const void*,size_t);
typedef void (*file_flush_t)(void*);
extern const char newline;

struct file_base{
    void* object;
    char* buffer;
    size_t buffer_size;
    size_t buffer_begin,buffer_end;
    file_write_t write;
    file_read_t read;
    file_flush_t flush;
};
typedef struct file_base file_t;
//初期化に関する関数
void file_base_init(file_t* file,size_t size);//バッファの構築
//書き込みに関する関数
void file_printf(file_t* ifile,const char* format,...);
size_t file_write(file_t* ifile,const void* mem,size_t size);
size_t file_puts(file_t* ifile,const char* text);
size_t file_putc(file_t* ifile,char c);
//読み込みに関する関数
size_t file_read(file_t* ifile,void* mem,size_t size);
char* file_gets(file_t* ifile,char* mem,size_t size);
char file_getc(file_t* ifile);

//特殊操作
void file_flush(file_t* file);


#endif