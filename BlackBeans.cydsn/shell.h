#pragma once

void shell_init();
void shell_process();

void shell_puts(const char*);
void shell_system(char*);
void shell_run(int argc,char** argv);