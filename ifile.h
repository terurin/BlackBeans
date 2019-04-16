#pragma once
#ifndef __IFILE_HEADER_GUARD__
#define __IFILE_HEADER_GUARD__
#include <stddef.h>


struct ifile{
    void* object;
    size_t (*write)(void* object,const void* mem,size_t);
    size_t (*read)(void* object,void* mem,size_t);
    size_t (*write_back)(void* object,void* mem,size_t);//readのキャンセルに用いる
    size_t (*flush)(void* object);
};
typedef struct ifile ifile_t;

//書き込みに関する関数
size_t ifile_printf(ifile_t* ifile,const char* format,...);
size_t ifile_write(ifile_t* ifile,const void* mem,size_t size);
size_t ifile_puts(ifile_t* ifile,const char* text);
size_t ifile_putc(ifile_t* ifile,char c);
//読み込みに関する関数
size_t ifile_read(ifile_t* ifile,void* mem,size_t size);
char* ifile_gets(ifile_t* ifile,char* mem,size_t size);
char ifile_getc(ifile_t* ifile);
size_t ifile_write_back(ifile_t* object,void* mem,size_t);
//特殊操作
size_t ifile_flush(ifile_t* ifile);


#endif