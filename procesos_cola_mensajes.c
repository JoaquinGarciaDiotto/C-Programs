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

#define delay 5
#define delayClasificador 3
#define delayReciclador 6
#define cantResiduos 4

const char Residuos[cantResiduos] = {'v','c','p','a'};
/*
* Tipo 1: De Reciclador a Clasificador. El resto se pasa el Char de su tipo a long.
*/

struct message {
	long type;
	char residuo;
};
const int size = sizeof(struct message)-sizeof(long);

void recolector(int i, int idRC);
void clasificador(int i, int id);
void reciclador(char r, int id);

int main(){

  	// Crear cola de mensajes
  	key_t idkey;
  
  	idkey = ftok("proyecto1b.c", 'k');

	if(idkey==-1){
		perror("ftok()");
		exit(1);
	}

  	int id = msgget(idkey, IPC_CREAT|0666);

	if(id==-1){
		perror("Error al crear cola");
		exit(1);
	}


  	
    //Crear los Recolectores
    for(int i = 0; i<3; i++){
        int pid = fork();
        if(pid==0) {
           recolector(i, id);
        } else if(pid==-1){
			printf("Error al crear recolector %i\n",i);
			perror("fork()");
			exit(1);
		}
    }

    //Crear los Clasificadores
    for(int i = 0; i<2; i++){
        int pid = fork();
        if(pid==0) {
            clasificador(i,id);
			
        } else if(pid==-1){
			printf("Error al crear clasificador %i\n",i);
			perror("fork()");
			exit(1);
		}
    }
  
	//Crea los recicladores
  	for(int i = 0; i<4; i++){
      	int pid = fork();
      	if(pid==0) {
          	reciclador(Residuos[i],id);
        } else if(pid==-1){
			printf("Error al crear reciclador %i\n",i);
			perror("fork()");
			exit(1);
		}
    }
	waitpid(0,NULL,0);
    msgctl(id,IPC_RMID,NULL);		//Intentar cerrar la cola al finalizar, pero no funciona ya que se debe crashear el programa y nunca se ejecuta, se deben eliminar manualmente o reiniciando el equipo.
	return 0;
}

void recolector(int i, int id){
  	srand(getpid()*time(NULL));
    while(1){
        int r = rand()%4;
        char resi = Residuos[r];
        printf("Recolectando %d %c\n",i,resi);
        fflush(stdout);
		struct message m = {1,resi};
		if(msgsnd(id,&m,size,0)==-1){
			printf("Error al escribir mensaje %s, Recolector %d\n",resi,i);
			perror("msgsnd");
			exit(1);
		}
		sleep(delay);
    }
}

void clasificador(int i, int id){
	sleep(1);
  	while(1) {
		struct message m;
		if(msgrcv(id,&m,size,1,0)==-1){
			printf("Error al leer mensaje %s\n",m.residuo);
			perror("msgrcv");
			exit(1);
		}
		char res = m.residuo;
		m.type = res;			//Le cambiamos el 'type' a el tipo de residuo.
		printf("Clasificando %d %c\n",i , res);
		fflush(stdout);
		if(msgsnd(id,&m,size,0)==-1){
			printf("Error al escribir mensaje %s, Clasificador %d\n",res,i);
			exit(1);
		}
		sleep(delayClasificador);
    }
}

void reciclador(char tipo, int id){
  	struct message m;
    srand(getpid()*time(NULL));
	sleep(2);
  	while(1) {
      	if (msgrcv(id,&m,size,tipo,IPC_NOWAIT)==-1) {    //Se fija si tiene algo para reciclar, sino hace otra cosa
            int r = rand()%2;   //Decide al azar si tomar mate o ayudar al resto
            if(r==0)
                printf("Reciclador %c esta tomando mate\n",tipo);
			else {
				char tipoTemp = ' ';
				for(int i = 0; i<cantResiduos; i++){
					tipoTemp = Residuos[i];
					if(tipoTemp!=tipo)
						if(msgrcv(id,&m,size,(long)tipoTemp,IPC_NOWAIT)!=-1){
							printf("Reciclando-Ayudo %c: %c\n",tipo,tipoTemp);
							break;		//Solamente colabora con un reciclador
						}
				}
                
            }
			sleep(1);
		} else 
      		printf("Reciclando-%c: %c\n", tipo, m.residuo);
      	fflush(stdout);
		sleep(delayReciclador);
    }
}
