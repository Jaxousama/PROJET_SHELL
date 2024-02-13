#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"


void command_quit(){
    exit(1);
}

int command_general(char** cmd){
    pid_t pid;
    int err;
    pid=Fork();
    if(pid==-1){
        printf("error : Fork Issue");
        exit(1);
    }
    if(pid==0){ //Fiston
        err = execvp(cmd[0],&cmd[0]);
        if(err<0){
            printf("%s : command not found\n",cmd[0]);
        }
        exit(0);
    }
    else{ //Pap's
        pid = waitpid(pid,NULL,0);
        return 0;
    }
    return 0;
}

int command_redirection(char* out,char* in,char** cmd){
    pid_t pid;
    int err;
    pid=Fork();
    if(pid==-1){
        printf("error : Fork Issue");
        exit(1);
    }
    if(pid==0){ // Daughter
        if(out){
            int fd_out;
            fd_out=Open(out, O_CREAT | O_WRONLY, 0644);
            dup2(fd_out,1);
        }
        if(in){
            int fd_in;
            fd_in=Open(in, O_CREAT | O_RDONLY, 0644);
            dup2(fd_in,0);
        }
        err = execvp(cmd[0],&cmd[0]);
        if(err<0){
            printf("%s : command not found\n",cmd[0]);
        }
        exit(0);
    }
    else{ //Mommy
        pid = waitpid(pid,NULL,0);
        return 0;
    }
    return 0;
}