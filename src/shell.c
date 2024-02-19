/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readcmd.h"
#include "csapp.h"
#include "command.h"



int main()
{
	while (1) {
		struct cmdline *l;
		int i, j,perr,flag_dernier,flag_pipe,pid;
		char pipe_name[TAILLE_MAX];
		char pipe_num[TAILLE_MAX];
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
			printf("errorrrrr: %s\n", l->err);
			continue;
		}

		/* Display each command of the pipe */
		for (i=0; l->seq[i]!=0; i++) {
			strcpy(pipe_name,"pipe");
			sprintf(pipe_num,"%d",i);
			strcat(pipe_name,pipe_num);
			if(l->seq[i+1]!=0){
				perr=mkfifo(pipe_name,0644);
				if(perr<0){
					printf("error pipe");
					exit(0);
				}
				flag_dernier=0;
				flag_pipe=1;
			}
			else{
				flag_dernier = 1;
			}
			char **cmd = l->seq[i];
			printf("seq[%d]: ", i);
			for (j=0; cmd[j]!=0; j++) {
				printf("%s ", cmd[j]);
			}
			command_redirection(l->out,l->in,cmd,i,flag_dernier);
			printf("\n");
		}
		while((pid = waitpid(-1,NULL,0))>0){}
	}
}
