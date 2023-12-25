#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mutexNorte, mutexSur, cantNorte, cantSur, puenteLibre;
#define CantNorte 2
#define CantSur 2
#define DELAY 1 //Delay de los autos para cruzar el puente

void VehiculoNorte() {
    while(1) {
        sem_wait(&mutexNorte);              //Exclusion mutua para cada vehiculo
        if (sem_trywait(&cantNorte)==0)       //Determinar si estan pasando los de mi direccion
            sem_post(&cantNorte);
        else 
            sem_wait(&puenteLibre);     //Sino, soy el primero y tengo que esperar.
        sem_post(&cantNorte);       //Habilito el paso a el proximo auto
        sem_post(&mutexNorte);

        printf(" | Cruza vehiculo proveniente del Norte | ");
        fflush(stdout);
        sleep(DELAY);

        sem_wait(&mutexNorte);
        sem_wait(&cantNorte);
        if(sem_trywait(&cantNorte)==0){     //Si soy el ultimo, habilito el puente
            sem_post(&cantNorte);
            sem_post(&puenteLibre);
        }
        sem_post(&mutexNorte);
    }
    exit(0);
}

void VehiculoSur() {
    while(1) {
        sem_wait(&mutexSur);
        if (sem_trywait(&cantSur)==0) 
            sem_post(&cantSur);
        else 
            sem_wait(&puenteLibre);
        sem_post(&cantSur);
        sem_post(&mutexSur);

        printf(" | Cruza vehiculo proveniente del Sur | ");
        fflush(stdout);
        sleep(DELAY);

        sem_wait(&mutexSur);
        sem_wait(&cantSur);
        if(sem_trywait(&cantSur)==0){
            sem_post(&cantSur);
            sem_post(&puenteLibre);
        }
        sem_post(&mutexSur);
    }
    exit(0);
}

int main() {
    sem_init(&mutexNorte,1,1);
    sem_init(&mutexSur,1,1);
    sem_init(&cantNorte,1,0);
    sem_init(&cantSur,1,0);
    sem_init(&puenteLibre,1,1);

    pthread_t thread[CantNorte+CantSur];
    for(int i = 0; i<CantNorte; i++)
        pthread_create(&(thread[i]),NULL,(void*)VehiculoNorte,NULL);
    for(int i = 0; i<CantSur; i++)
        pthread_create(&(thread[i+CantNorte]),NULL,(void*)VehiculoSur,NULL);

    for (int i=0;i<4;i++){
        pthread_join(thread[i],NULL);
    }

    return 0;
}
