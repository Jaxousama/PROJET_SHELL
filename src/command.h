#ifndef __COMMAND_H__
#define __COMMAND_H__

void command_quit();

int command_general(char** cmd);

int command_redirection(char* out,char* in,char** cmd);

#endif 