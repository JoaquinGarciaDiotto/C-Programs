#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define C 3
#define N 15

sem_t asientosDisp,adentro,mutex_taxi;

void* pasajero(void* p){
    printf("pasajero %d: Quiero viajar\n",(int*)p);
    sem_wait(&asientosDisp);
    sem_wait(&mutex_taxi);
    printf("pasajero %d: Entre al auto, espero a que se llene\n",(int*)p);
    //Adentro del auto
    sem_post(&adentro);
    sem_post(&mutex_taxi);
}

void* taxi(){
  
    int i=0;
    while(1){
      for(i=0;i<C;i++){
	sem_wait(&adentro);
	printf("taxi: llego un pasajero, faltan %d\n",(C-1-i));
      }
	sem_wait(&mutex_taxi);
	printf("taxi: Llegaron todos los pasajeros, empieza el viaje\n");
      //Empieza el viaje
	printf("taxi: Termino el viaje\n");
      for(i=0;i<C;i++){
	sem_post(&asientosDisp);
      }
	sem_post(&mutex_taxi);
    } 
}

int main(){
    
    int j;
  
    pthread_t vehiculo,persona[N];
  
    sem_init(&asientosDisp,0,C);
    sem_init(&adentro,0,0);
    sem_init(&mutex_taxi,0,1);
    
    for(j=0;j<N;j++){
      pthread_create(&persona[j],NULL,pasajero,(void *)j);
    }
    
    pthread_create(&vehiculo,NULL,taxi,NULL);
    
    for(j=0;j<N;j++){
      pthread_join(persona[j],NULL);
    }
      pthread_join(vehiculo,NULL);
    
}

