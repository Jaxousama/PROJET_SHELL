#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "command.h"
#include "csapp.h"


void command_quit(){
    //On supprime Le dossier qui contient les pipes
    //et comme on exec une commande le reste ne sera pas executé
    execl("/bin/rm","rm","-r","Pipe",NULL);
}

void Interrup(int sig){//gestion SIGINT pour les fils
	exit(0);
}

int command_redirection(char* out,char* in,char** cmd,int nombre_cmd,int flag_dernier,int flag_background){
    pid_t pid;
    int err,i,fd_in,fd_out,errset;
    int fd_pipeout,fd_pipein;
    char pipe_name[TAILLE_MAX];
    char pipe_num[TAILLE_MAX];

    //on recupere le nom du pipe
    strcpy(pipe_name,"Pipe/pipe");
	sprintf(pipe_num,"%d",nombre_cmd);
	strcat(pipe_name,pipe_num);

    //on regarde si ya un quit
    for (i=0; cmd[i]!=0; i++) {
		if(!(strcmp(cmd[i],"quit"))){
		printf("\n");
		command_quit();
	    }
    }

    pid=Fork();// on cree un fils
    if(pid==-1){
        printf("error : Fork Issue \n");
        exit(1);
    }
    
    if(pid==0){ // Child
        Signal(SIGINT, Interrup);//Changement de SIGINT pour les childs
        
        if((!flag_dernier) && (nombre_cmd==0)){//si c'est la derniere commande du pipe
            fd_pipeout=open(pipe_name,O_WRONLY, 0644);
            dup2(fd_pipeout,1);
        }
        if(nombre_cmd>0){ //si ce n'est pas la premiere commande 
            if(!flag_dernier){
                fd_pipeout=open(pipe_name,O_WRONLY, 0644);
                dup2(fd_pipeout,1);
            }
            strcpy(pipe_name,"/tmp/pipe");
            sprintf(pipe_num,"%d",nombre_cmd-1);
            strcat(pipe_name,pipe_num);
            fd_pipein=open(pipe_name,O_RDONLY, 0644);
            dup2(fd_pipein,0);
            
        }
        if(out){//si il y a une redirection de la sortie
            fd_out=open(out, O_CREAT | O_WRONLY | O_TRUNC , 0644);
            if(fd_out < 0){
                printf("%s : Permission denied \n",out);
                exit(1);
            }
            dup2(fd_out,1);
        }
        if(in){//si il y a une redirection de l'entrée
            fd_in=open(in,O_RDONLY, 0644);
            if(fd_in < 0){
                printf("%s : Permission denied \n",in);
                exit(1);
            }
            dup2(fd_in,0);
        }
        err = execvp(cmd[0],cmd); //on execute la commande 
        if(err<0){//si la commande c'est mal passé
            printf("%s : command not found \n",cmd[0]);
            exit(1);
        }

        //fermeture des pipes et fichiers ouvert precedemment
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

    else{ //Father
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
