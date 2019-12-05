#pragma once

void shell_init();
void shell_process();

void shell_puts(const char*);
void shell_system(char*);
void shell_run(int argc,char** argv);

void shell_error(int,char**);//default error
void shell_echo(int,char**);
void shell_help(int,char**);
void shell_list(int,char**);
void shell_last(int,char**);
