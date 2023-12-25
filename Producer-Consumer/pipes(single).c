#define _GNU_SOURCE
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
#include <semaphore.h>
#include <fcntl.h>

#define N 10
#define delay 1
#define L 0
#define E 1

int buffer[2]; //Hace falta 2 pipes mas? 1 para comunicar desde el Producto al Consumidor y el otro para viceversa?
int PSC[2], CSP[2]; //De cualquier forma, en ningun caso creo que importaria hacer que sean no bloqueantes.

void Productor(){
    close(buffer[L]);
    close(PSC[L]);
    close(CSP[E]);
    srand(getpid());
    int aux = 0;
    while(1){
        read(CSP[L],&aux,1);    //Se queda bloqueado hasta que le manden un 'signal'.

        aux = rand();
        printf("Produciendo %i\n",aux);
        fflush(stdout);
        write(buffer[E],&aux,sizeof(aux));
        write(PSC[E],&aux,1);       //Avisamos que salimos de la SC

        sleep(delay);
    }
}
void Consumidor(){
    close(buffer[E]);
    close(PSC[E]);
    close(CSP[L]);
    int aux = 0;
    while(1){
        read(PSC[L],&aux,1);
        
        read(buffer[L],&aux,sizeof(aux));
        printf("Se consumio: %i\n",aux);
        fflush(stdout);
        write(CSP[E],&aux,1);

        sleep(delay);
    }
}


int main(){
    if(pipe(buffer)<0){
        perror("pipe B");
        exit(1);
    }
    if(pipe(PSC)<0){
        perror("pipe PSC");
        exit(1);
    }
    if(pipe(CSP)<0){
        perror("pipe CSP");
        exit(1);
    }
    if(fcntl(buffer[1],F_SETPIPE_SZ,10)<0){     //Le seteamos el size a 10.
        perror("fnctl");
        exit(1);
    }
   
    int pidP = fork();
    if(pidP==-1){
        perror("fork P");
        exit(1);
    }
    else if (pidP==0)
        Productor();
    
    int pidC = fork();
    if(pidC==-1){
        perror("fork C");
        exit(1);
    }
    else if (pidC==0)
        Consumidor();

    int temp = 0;
    write(CSP[E],&temp,1);

    close(buffer[E]);
    close(buffer[L]);
    close(PSC[E]);
    close(CSP[E]);
    close(PSC[L]);
    close(CSP[L]);

    waitpid(pidP,NULL,0);
    waitpid(pidC,NULL,0);
    return 0;
}

/*
Si se cuenta con 3 P y 2 C, no cambiaria nada, ya que se seguirian sincronizando mediante los pipes existentes
y solo un productor o consumidor podria entrar a la SC a la vez, por mas que existan mas de uno de cada uno.
*/