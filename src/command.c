#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "command.h"
#include "csapp.h"


void command_quit(){
    execl("/bin/rm","rm","-r","Pipe",NULL);
}


int command_redirection(char* out,char* in,char** cmd,int nombre_cmd,int flag_dernier,int flag_background){
    pid_t pid;
    int err,i,fd_in,fd_out,errset;
    int fd_pipeout,fd_pipein;
    char pipe_name[TAILLE_MAX];
    char pipe_num[TAILLE_MAX];
    strcpy(pipe_name,"Pipe/pipe");
	sprintf(pipe_num,"%d",nombre_cmd);
	strcat(pipe_name,pipe_num);
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
        if((!flag_dernier) && (nombre_cmd==0)){
            fd_pipeout=open(pipe_name,O_WRONLY, 0644);
            dup2(fd_pipeout,1);
        }
        if(nombre_cmd>0){
            if(!flag_dernier){
                fd_pipeout=open(pipe_name,O_WRONLY, 0644);
                dup2(fd_pipeout,1);
            }
            strcpy(pipe_name,"Pipe/pipe");
            sprintf(pipe_num,"%d",nombre_cmd-1);
            strcat(pipe_name,pipe_num);
            fd_pipein=open(pipe_name,O_RDONLY, 0644);
            dup2(fd_pipein,0);
            
        }
        if(out){
            fd_out=open(out, O_CREAT | O_WRONLY | O_TRUNC , 0644);
            if(fd_out < 0){
                printf("%s : Permission denied \n",out);
                exit(1);
            }
            dup2(fd_out,1);
        }
        if(in){
            fd_in=open(in,O_RDONLY, 0644);
            if(fd_in < 0){
                printf("%s : Permission denied \n",in);
                exit(1);
            }
            dup2(fd_in,0);
        }
        err = execvp(cmd[0],cmd);
        if(err<0){
            printf("%s : command not found \n",cmd[0]);
            exit(1);
        }
        if(!flag_dernier){
            close(fd_pipeout);
        }
        if(!(!flag_dernier) && (nombre_cmd==0)){
            close(fd_pipein);
        }
        if(out){
            close(fd_out);
        }
        if(in){
            close(fd_in);
        }
        exit(0);
    }

    else{ //Mommy
        if(flag_background){
            errset=setpgid(pid,0);
            if(errset<0){
                printf("error : setpgid");
            }
        }
        return 0;
    }
    return 0;
}
