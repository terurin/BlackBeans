#include "file_base.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <FreeRTOS.h>
const char newline='\n';
static inline size_t min(size_t x,size_t y){
    return x<y?x:y;
}

void file_base_init(file_t* file,size_t size){
    assert(file);
    assert(size>0);
    memset(file,0,sizeof(file_t));
    file->buffer=(char*)pvPortMalloc(size*sizeof(char));    
    file->buffer_size=size;
}

void file_printf(file_t* file, const char* format, ...) {
    assert(file&&file->write);
    assert(format);
    va_list args;
    va_start(args, format);
    char buffer[20];
    vsprintf(buffer,format,args);
    file->write(file->object,buffer,strlen(buffer));
    va_end(args);
    return ;
}

size_t file_write(file_t* ifile, const void* mem, size_t size) {
    assert(ifile&&ifile->write);
    assert(mem);
    return ifile->write(ifile->object, mem, size);
}

size_t file_puts(file_t* ifile, const char* text) {
    assert(ifile&&ifile->write);
    assert(text);
    return ifile->write(ifile->object, text, strlen(text));
}

size_t file_putc(file_t* ifile, char c) {
    assert(ifile&&ifile->write);
    return ifile->write(ifile->object, &c, sizeof(c));
}

size_t file_read(file_t* file, void* mem, size_t size) {
    assert(file&&file->read);
    assert(mem);
    //bufferから読み出す
    const size_t used=file->buffer_end-file->buffer_begin;
    size_t read_size=min(size,used);
    memcpy(mem,&file->buffer[file->buffer_begin],read_size);
    if (read_size<=size){
        file->buffer_begin+=read_size;
        return read_size;
    }
    //不足分を読み出す
    return file->read(file->object,&mem[read_size],size-read_size)+read_size;
}

char* file_gets(file_t* file, char* str, size_t size) {
    assert(file&&file->read&&file->read);
    assert(str);
    //バッファを切り詰める
    const size_t used_size=file->buffer_end-file->buffer_begin;
    memmove(&file->buffer[0],&file->buffer[file->buffer_begin],used_size);
    file->buffer_begin=0;
    file->buffer_end=used_size;
    //バッファーを書き足す
    const size_t last_size=file->buffer_size-used_size;
    file->buffer_end+=file->read(file->object,
        &file->buffer[file->buffer_end],last_size);
    //バッファーに改行があるか?
    const size_t end =min( file->buffer_end,size);
    size_t pos=0;
    for (size_t i=0;i<end;i++){
        if (file->buffer[i]==newline){
            file->buffer[i]=0;
            memcpy(str,file->buffer,i);
            file->buffer_begin=i;
            return str;
        }
    }
    //未発見
    return NULL;
}

char file_getc(file_t* file) {
    assert(file&&file->read);
    //バッファーに要素がない
    if (file->buffer_begin>=file->buffer_end){
        //バッファーを更新
        size_t end=file->read(file->object,file->buffer,file->buffer_size); 
        file->buffer_begin=0;
        file->buffer_end=end;
        //やはりない
        if (!end){
            return '\0';
        }
    }
    
    return file->buffer[file->buffer_begin++];
}

void file_flush(file_t* file){
    assert(file);
    file->buffer_begin=file->buffer_end=0;
    return ifile->flush(file->object);
}
