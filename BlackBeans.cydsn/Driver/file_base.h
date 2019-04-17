#pragma once
#ifndef __FILE_BASE_HEADER_GUARD__
#define __FILE_BASE_HEADER_GUARD__
#include <stddef.h>


struct file_base{
    void* object;
    size_t (*write)(void* object,const void* mem,size_t);
    size_t (*read)(void* object,void* mem,size_t);
    size_t (*write_back)(void* object,void* mem,size_t);//readのキャンセルに用いる
    size_t (*flush)(void* object);
};
typedef struct file_base file_t;

//書き込みに関する関数
size_t ifile_printf(file_t* ifile,const char* format,...);
size_t ifile_write(file_t* ifile,const void* mem,size_t size);
size_t ifile_puts(file_t* ifile,const char* text);
size_t ifile_putc(file_t* ifile,char c);
//読み込みに関する関数
size_t ifile_read(file_t* ifile,void* mem,size_t size);
char* ifile_gets(file_t* ifile,char* mem,size_t size);
char ifile_getc(file_t* ifile);
size_t ifile_write_back(file_t* object,void* mem,size_t);
//特殊操作
size_t file_flush(file_t* file);


#endif