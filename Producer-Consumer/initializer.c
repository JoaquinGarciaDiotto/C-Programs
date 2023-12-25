#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <string.h>
#include <time.h>


#define N 30
#define SEGSIZE sizeof (struct info)
#define KEY ((key_t)(1243))
#define delay 1
#define True 1
#define False 0
#define P 0
#define C 1

struct info {
    int buffer[N];
    int cursor;
    int turno;
    int flag[2];
};

int main(){

    int id = shmget(KEY,SEGSIZE,IPC_CREAT | 0666);
    if(id==-1){
        perror("shmget");
        exit(1);
    }
    struct info* ctrl = (struct info*)shmat(id,NULL,0);
    if(ctrl==(struct info*)(-1)){
        perror("shmat P");
        exit(1);
    }

    ctrl->cursor = 0;
    ctrl->turno = 0;
    (ctrl->flag)[0] = False;
    (ctrl->flag)[1] = True;

   
    int pidP = fork();
    if(pidP==0){
        char ** args = (char**)malloc(sizeof(char)*50);
        args[0] = "producer\0";
        args[1] = NULL;
        if(execv(args[0],args)==-1){
            perror("execv P");
            exit(1);
        }
    }else if (pidP==-1){
      perror("fork P");
      exit(1);
    }
    int pidC = fork();
    if(pidC==0){
        char ** args = (char**)malloc(sizeof(char)*50);
        args[0] = "consumer\0";
        args[1] = NULL;
        if(execv(args[0],args)==-1){
            perror("execv C");
            exit(1);
        }
    }else if (pidC==-1){
        perror("fork C");
        exit(1);
    }

    waitpid(pidP,NULL,0);
    waitpid(pidC,NULL,0);
    return 0;
}

/*
Si se cuenta con 3 P y 2 C, habria que cambiar el algoritmo de sincronizacion, utilizando el algoritmo del Panadero
o con Semaforos.
*/