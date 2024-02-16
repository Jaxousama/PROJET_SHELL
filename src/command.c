#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"


void command_quit(){
    exit(1);
}

/*
int command_general(char** cmd){
    pid_t pid;
    int err,i;
    for (i=0; cmd[i]!=0; i++) {
		if(!(strcmp(cmd[i],"quit"))){
		printf("\n");
		command_quit();
	    }
    }
    pid=Fork();
    if(pid==-1){
        printf("error : Fork Issue \n");
        exit(1);
    }
    if(pid==0){ //Fiston
        err = execvp(cmd[0],&cmd[0]);
        if(err<0){
            printf("%s : command not found \n",cmd[0]);
        }
        exit(0);
    }
    else{ //Pap's
        pid = waitpid(pid,NULL,0);
        return 0;
    }
    return 0;
}
*/
int command_redirection(char* out,char* in,char** cmd){
    pid_t pid;
    int err,i;
    for (i=0; cmd[i]!=0; i++) {
		if(!(strcmp(cmd[i],"quit"))){
		printf("\n");
		command_quit();
	    }
    }
    pid=Fork();
    if(pid==-1){
        printf("error : Fork Issue \n");
        exit(1);
    }
    if(pid==0){ // Daughter
        if(out){
            int fd_out;
            fd_out=open(out, O_CREAT | O_WRONLY | O_TRUNC , 0644);
            if(fd_out < 0){
                printf("%s : Permission denied \n",out);
                exit(1);
            }
            dup2(fd_out,1);
        }
        if(in){
            int fd_in;
            fd_in=open(in,O_RDONLY, 0644);
            if(fd_in < 0){
                printf("%s : Permission denied \n",in);
                exit(1);
            }
            dup2(fd_in,0);
        }
        err = execvp(cmd[0],&cmd[0]);
        if(err<0){
            printf("%s : command not found \n",cmd[0]);
            exit(1);
        }
        exit(0);
    }
    else{ //Mommy
        pid = waitpid(pid,NULL,0);
        return 0;
    }
    return 0;
}