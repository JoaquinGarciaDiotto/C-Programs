#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

#define Lect 0
#define Escr 1
#define DELAY 5
#define DELAYCLASIFICADOR 3
#define DELAYRECICLADOR 6
#define cantResiduos 4
const char Residuos[cantResiduos] = {'v','c','p','a'};

void recolector(int i);
void clasificador(int i);
void reciclador(char tipo, int pfd[2],  int *pipesResiduos[cantResiduos]);

//Creacion de las pipes
int pRC[2], pCRV[2], pCRC[2], pCRP[2], pCRA[2];


int main(){

    if (pipe(pRC) < 0) {
        perror("pipe() pRC");
        exit(1);
    }

    if (pipe(pCRV) < 0) {
        perror("pipe() pCRV");
        exit(1);
    }

    if (pipe(pCRC) < 0) {
        perror("pipe() pCRC");
        exit(1);
    }

    if (pipe(pCRP) < 0) {
        perror("pipe() pCRP");
        exit(1);
    }

    if (pipe(pCRA) < 0) {
        perror("pipe() pCRA");
        exit(1);
    }

    if (fcntl(pCRV[Lect], F_SETFL, O_NONBLOCK)) {
        perror("fcntl() pCRV");
        exit(1);
    }

    if (fcntl(pCRC[Lect], F_SETFL, O_NONBLOCK)) {
        perror("fcntl() pCRV");
        exit(1);
    }

    if (fcntl(pCRP[Lect], F_SETFL, O_NONBLOCK)) {
        perror("fcntl() pCRP");
        exit(1);
    }

    if (fcntl(pCRA[Lect], F_SETFL, O_NONBLOCK)) {
        perror("fcntl() pCRA");
        exit(1);
    }

    //Crear los Recolectores
    for(int i = 0; i<3; i++){
        int pid = fork();
        if(pid==0) {
           recolector(i);
        }
    }

    //Crear los Clasificadores
    for(int i = 0; i<2; i++){
        int pid = fork();
        if(pid==0) {
            clasificador(i); 
        }
    }
  
	//Crea los recicladores
  	for(int i = 0; i<4; i++){
        int *pipesResiduos[cantResiduos] = {pCRV,pCRC,pCRP,pCRA};
      	int pid = fork();
      	if(pid==0) {
          	switch(i) {
              	case 0: reciclador('v',pCRV,pipesResiduos);
                				break;
              	case 1: reciclador('c',pCRC,pipesResiduos);
                				break;
              	case 2: reciclador('p',pCRP,pipesResiduos);
                				break;
             	case 3: reciclador('a',pCRA,pipesResiduos);
            }
        }
    }
    close(pRC[Lect]);
    close(pRC[Escr]);
    close(pCRA[Lect]);
    close(pCRA[Escr]);
    close(pCRC[Lect]);
    close(pCRC[Escr]);
    close(pCRP[Escr]);
    close(pCRP[Lect]);
    close(pCRV[Escr]);
    close(pCRV[Lect]);
    wait(NULL);
    return 0;
}

void recolector(int i){
    close(pRC[Lect]);
    close(pCRA[Lect]);
    close(pCRA[Escr]);
    close(pCRC[Lect]);
    close(pCRC[Escr]);
    close(pCRP[Escr]);
    close(pCRP[Lect]);
    close(pCRV[Escr]);
    close(pCRV[Lect]);
    char buf = ' ';
    int r = 0;
    srand(getpid());
    while(1){
        r = rand()%4;
        buf = Residuos[r];
        printf("Recolectando %d %c\n",i,buf);
        fflush(stdout);
        write(pRC[Escr],&buf,1);
        sleep(DELAY);
    }

    exit(0);
}

void clasificador(int i){
    close(pRC[Escr]);
    close(pCRV[Lect]);
    close(pCRC[Lect]);
    close(pCRP[Lect]);
    close(pCRA[Lect]);

    char buf = ' ';
    sleep(1);
    while(1) {
            read(pRC[Lect], &buf, 1);
            printf("Clasificando %d %c\n",i , buf);
            fflush(stdout);
            switch(buf) {
            case 'v': {
                    write(pCRV[Escr],&buf,1);
                    break;
            }
            case 'c': {
                    write(pCRC[Escr],&buf,1);
                    break;
            }
            case 'p': {
                    write(pCRP[Escr],&buf,1);
                    break;
            }
            case 'a':
                    write(pCRA[Escr],&buf,1);
        }
        sleep(DELAYCLASIFICADOR);
    }
    exit(0);
}

void reciclador(char tipo, int pfd[2], int *pipesResiduos[cantResiduos]){
    close(pRC[Lect]);
    close(pRC[Escr]);
    close(pCRA[Escr]);
    close(pCRC[Escr]);
    close(pCRP[Escr]);
    close(pCRV[Escr]);
    srand(getpid());
  	int rnd = 0;
  	char buf = ' ';
  	sleep(2);
  	while(1) { 
        ssize_t r = read(pfd[Lect], &buf, 1);
        if (r > 0) { // Intenta reciclar su material preferente
            printf("Reciclando-%c: %c\n", tipo, buf);
        } else { 
            rnd = rand()%2;     //Decide al azar si tomar mate o ayudar al resto.
            if(rnd==0) {    //Decide tomar mate
                printf("Reciclador %c toma mate\n",tipo);
            }
            else {      //Decide ayudar a los demas.
                char tipoTemp = ' ';
                for(int i = 0; i<cantResiduos; i++){
                    tipoTemp = Residuos[i];
                    if(tipoTemp!=tipo){
                        r = read(pipesResiduos[i][Lect], &buf, 1);
                        if (r > 0){
                            printf("Reciclando-Ayudo %c %c\n", tipo, buf);
                            break;      //Ayudamos a un unico reciclador
                        }
                    }
                }
            }
            sleep(1);
        }
        sleep(DELAYRECICLADOR);     //Para que tarden mas en reciclar
        fflush(stdout);
    }
    
    exit(0);
}
