#ifndef __COMMAND_H__
#define __COMMAND_H__

#define TAILLE_MAX 50

void command_quit();

int command_redirection(char* out,char* in,char** cmd,int nombre_cmd,int flag);

#endif 