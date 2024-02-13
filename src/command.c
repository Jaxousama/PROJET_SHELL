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