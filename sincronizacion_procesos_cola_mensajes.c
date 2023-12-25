#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <time.h>

#define DELAY 1 //Delay de auto cruzando el puente
#define CANTAUTOSMAX 10 //Cantidad maxima de autos que pueden pasar por un lado
#define SWITCHDELAY 10 //Tiempo en cambiar el turno de direccion de los autos

typedef struct message {
	long type;
}msg;
const int size = sizeof(msg)-sizeof(long);



void VehiculoNorte(int id){
  	while(1) {
       	// Lee mensaje de la cola Norte y pasa por el puente
       	msg m ;
       	if(msgrcv(id,&m,size,(long)'n',0)==-1){
			printf("Error al leer mensaje en Vehiculo Norte\n");
			exit(1);
		}
      	printf("| Cruza vehiculo proveniente del norte |");
      	fflush(stdout);
        sleep(DELAY);
    }
}

void VehiculoSur(int id){
  	while(1) {
      	// Lee mensaje de la cola Sur y pasa por el puente
      	msg m;
      	if(msgrcv(id,&m,size,(long)'s',0)==-1){
       		printf("Error al leer mensaje en Vehiculo Sur\n");
			exit(1);
        }
        printf("| Cruza vehiculo proveniente del sur |");
        fflush(stdout);
        sleep(DELAY);
    }
}

void Puente(int id){
  	while(1) {
      	msg m;
        for(int i = 0; i<CANTAUTOSMAX; i++){
            m = *(msg*)malloc(sizeof(msg));
            m.type = (long)'n';
            if(msgsnd(id,&m,size,0)==-1){
              	printf("Error al escribir mensaje en la cola Norte, iteracion %i\n",i);
              	perror("msgsnd");
                exit(1);
            }
        }
        sleep(SWITCHDELAY);
      	while(msgrcv(id,&m,size,(long)'n',IPC_NOWAIT)!=-1){} //Consumir mensajes que hayan podido quedar para autos del Norte. (Es decir, si no vienen mas autos)
      	for(int i = 0; i<CANTAUTOSMAX; i++){
            m = *(msg*)malloc(sizeof(msg));
            m.type = (long)'s';
            if(msgsnd(id,&m,size,0)==-1){
             	printf("Error al escribir mensaje en la cola Sur, iteracion %i\n",i);
            	perror("msgsnd");
                exit(1);
            }
        }
        sleep(SWITCHDELAY);
      	while(msgrcv(id,&m,size,(long)'s',IPC_NOWAIT)!=-1){}
    }
}

int main(){
  	key_t idkey;
    idkey = ftok("ejercicio2iii.c",'k');
    if(idkey==-1){
        printf("Error al crear la idkey\n");
        perror("ftok");
        exit(1);
    }


  	int id = msgget(idkey, IPC_CREAT|0666);
  	
    if(id==-1) {
        perror("Error al crear cola de mensajes");
        exit(1);
    }
  	// Creamos proceso Puente
  	int pidp = fork();
  	if(pidp==0){
     	Puente(id);
    } else if(pidp==-1){
        printf("Error al crear proceso Puente");
        perror("fork()");
        exit(1);
    }
  
  	// Creamos proceso de vehiculos con direccion Norte. Ver si hay que usar execv
    int pidn = fork();
    if(pidn==0){
        VehiculoNorte(id);
    } 
    else if(pidn==-1){
        printf("Error al crear proceso Norte");
        perror("fork()");
        exit(1);
    }
  	 // Creamos proceso de vehiculos con direccion Sur
    int pids = fork();
    if(pids==0){
        VehiculoSur(id);
    } 
    else if(pids==-1){
        printf("Error al crear proceso Sur");
        perror("fork()");
        exit(1);
    }
    waitpid(pidp,NULL,0);
    waitpid(pidn,NULL,0);
    waitpid(pids,NULL,0);
    
    return 0;
}


















