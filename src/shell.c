/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readcmd.h"
#include "csapp.h"
#include "command.h"

void terminaison(int sig){
	waitpid(-1, NULL, WNOHANG|WUNTRACED);
}

void ignorer(int sig){
	printf("\n");
}

int main()
{
	signal(SIGINT, ignorer);
	Signal(SIGCHLD, terminaison);
	while (1) {
		struct cmdline *l;
		int i, j,perr,flag_dernier,pid;
		char pipe_name[TAILLE_MAX];
		char pipe_num[TAILLE_MAX];
		char* pwd=getenv("PWD");
		char* user=getenv("USER");
		printf("%s:%s>",user,pwd);
		l = readcmd(); 
		mkdir("Pipe",0755);
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
			strcpy(pipe_name,"Pipe/pipe");
			sprintf(pipe_num,"%d",i);
			strcat(pipe_name,pipe_num);
			if(l->seq[i+1]!=0){
				perr=mkfifo(pipe_name,0644);
				if(perr<0 && errno!=EEXIST){
					printf("error : Pipe Issue\n");
					exit(0);
				}
				flag_dernier=0;
			}
			else{
				flag_dernier = 1;
			}
			char **cmd = l->seq[i];
			//printf("seq[%d]: ", i);
			fflush(stdout);
			command_redirection(l->out,l->in,cmd,i,flag_dernier,l->back);
			//printf("\n");
		}
		
		if(!(l->back)){
			while(waitpid(0,NULL,0)>0){};
		}
		
	}
}

