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
#include <semaphore.h>

#define N 5
#define KEY ((key_t) (1243))
#define SEGSIZE sizeof (struct info)

struct info {
    int matriz1[N][N];
    int matriz2[N][N];
    sem_t control[N];      //Realmente no hace falta, pero bue ya fue.
};

void sumarFila(int i);

int main(){
    int id = shmget(KEY,SEGSIZE,IPC_CREAT | 0666);
    if(id==-1){
        perror("shmget");
        exit(1);
    }
    struct info* ctrl = (struct info*)shmat(id,NULL,0);
    if(ctrl==(struct info*)(-1)){
        perror("shmat");
        exit(1);
    }


    int v = 0;
    for(int i = 0; i<N; i++)
        for(int j = 0; j<N; j++){
            (ctrl->matriz1)[i][j] = v;
            (ctrl->matriz2)[i][j] = v;
            v++;
        }
    
    for(int i = 0; i<N; i++)
        sem_init(&((ctrl->control)[i]),1,0);
    for (int i = 0; i<N; i++){
        int pid = fork();
        if(pid==0)
            sumarFila(i);
        else if (pid==-1){
            printf("Error en fork, proceso %i\n",i);
            perror("fork");
            exit(1);
        }
    }
    
    for(int i = 0; i<N; i++)
        sem_wait(&((ctrl->control)[i]));

    FILE *fp;
    int buff;
    for(int i = 0; i<N; i++){
        char nombre[15];
        snprintf(nombre,15,"suma(%i).txt",i);
        fp = fopen(nombre,"r");
        printf("Resultado Fila %i: ",i);
        fflush(stdout);
        while(fscanf(fp,"%d\n",&buff) != EOF)
            printf("%d ",buff);
        printf("\n");
        fclose(fp);
    }
    shmctl(id,IPC_RMID,NULL);
    return 0;
}

void sumarFila(int i){
    int id = shmget(KEY,SEGSIZE,IPC_CREAT | 0666);  //Si no esta creado, lo crea.
    if(id==-1){
        perror("shmget");
        exit(1);
    }
    struct info* ctrl = (struct info*)shmat(id,NULL,0);
    if(ctrl==(struct info*)(-1)){
        perror("shmat");
        exit(1);
    }

    FILE *fp;
    char nombre[15];
    snprintf(nombre,15,"suma(%i).txt",i);
    fp = fopen(nombre,"w");

    int num1, num2, res;
    for(int j = 0; j<N; j++){
        num1 = (ctrl->matriz1)[i][j];
        num2 = (ctrl->matriz2)[i][j];
        res = num1 + num2;
        fprintf(fp,"%d\n",res);
    }

    fclose(fp);
    sem_post(&((ctrl->control)[i]));
    shmdt(ctrl);
    exit(0);
}