/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readcmd.h"
#include "csapp.h"
#include "command.h"

#define TAILLE_MAX 50

int main()
{
	while (1) {
		struct cmdline *l;
		int i, j,perr;
		char* pipe_name=malloc(sizeof(char)*TAILLE_MAX);
		char* pipe_num=malloc(sizeof(char)*TAILLE_MAX);
		char* pwd=getenv("PWD");
		char* user=getenv("USER");
		printf("%s:%s> ",user,pwd);
		l = readcmd(); 
		/* If input stream closed, normal termination */
		if (!l) {
			printf("exit\n");
			exit(0);
		}

		if (l->err) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
		}

		/* Display each command of the pipe */
		for (i=0; l->seq[i]!=0; i++) {
			pipe_name="pipe";

			sprintf(pipe_num,"%d",i);
			
			strcat(pipe_name,pipe_num);
			perr=mkfifo(pipe_name,0644);
			if(perr<0){
				printf("error pipe");
				exit(0);
			}
			char **cmd = l->seq[i];
			printf("seq[%d]: ", i);
			for (j=0; cmd[j]!=0; j++) {
				printf("%s ", cmd[j]);
			}
			command_redirection(l->out,l->in,cmd);
			printf("\n");
		}
	
	}
}
