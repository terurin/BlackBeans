#include "ifile.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

size_t ifile_printf(ifile_t* ifile, const char* format, ...) {
    assert(ifile&&ifile->write);
    assert(format);
    va_list args;
    va_start(args, format);
    char buffer[32];
    size_t size = vsprintf_s(buffer, sizeof(buffer), format, args);
    ifile->write(ifile->object, buffer, size);
    va_end(args);
    return size;
}

size_t ifile_write(ifile_t* ifile, const void* mem, size_t size) {
    assert(ifile&&ifile->write);
    assert(mem);
    return ifile->write(ifile->object, mem, size);
}

size_t ifile_puts(ifile_t* ifile, const char* text) {
    assert(ifile&&ifile->write);
    assert(text);
    return ifile->write(ifile->object, text, strlen(text));
}

size_t ifile_putc(ifile_t* ifile, char c) {
    assert(ifile&&ifile->write);
    return ifile->write(ifile->object, &c, sizeof(c));
}

size_t ifile_read(ifile_t* ifile, void* mem, size_t size) {
    assert(ifile&&ifile->read);
    assert(mem);
    return ifile->read(ifile->object, mem, size);
}

char* ifile_gets(ifile_t* ifile, char* str, size_t size) {
    assert(ifile&&ifile->read&&ifile->write_back);
    assert(str);
    const size_t size_read=ifile->read(ifile->object,str,size-1);
    size_t i;
    //改行があるか(TODO 要検証)
    for (i=0;i<size_read;i++){
        if (str[i]=='\n'){
            str[i]='\0';
            ifile->write_back(ifile->object,&str[i+1],size_read-i-1);
            return str;
        }
    }
    //破棄
    ifile->write_back(ifile->object,&str[i+1],size_read);
    return NULL;
}

char ifile_getc(ifile_t* ifile) {
    assert(ifile&&ifile->read);
    char r;
    ifile->read(ifile->object, &r,sizeof(r));
    return r;
}

size_t ifile_write_back(ifile_t* ifile,void* mem,size_t size){
    assert(ifile&&ifile->write_back);
    assert(mem);
    return ifile->write_back(ifile->object,mem,size);
}

size_t ifile_flush(ifile_t* ifile){
    assert(ifile);
    return ifile->flush(ifile->object);
}
