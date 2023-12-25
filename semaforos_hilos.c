#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semA, semB;


void * CalcularA(void* arreglo){
		int arregloReal[100] = (int*)arreglo;
		int arregloAuxiliar[50];
		for(int i = 0; i < 200; i++) {
    	for(int j = 0; j<50; j++)
        if(j==0)
        	arregloAuxiliar[0] = (arregloReal[99]+arregloReal[0]+arregloReal[1])/3;
        else
        	arregloAuxiliar[j] = (arregloReal[j-1]+arregloReal[j]+arregloReal[j+1])/3;
      sem_post(&semB);
      wait(&semA);
      for(int j = 0; j<50; j++)
      	arregloReal[j] = arregloAuxiliar[j];
      sem_post(&semB);
      wait(&semA);
    }
}
void * CalcularB(void* arreglo){
		int arregloReal[100] = (int*)arreglo;
		int arregloAuxiliar[50];
		for(int i = 0; i < 200; i++) {
    	for(int j = 0; j<50; j++)
        if(j==49)
        	arregloAuxiliar[49] = (arregloReal[98]+arregloReal[99]+arregloReal[0])/3;
        else
        	arregloAuxiliar[j] = (arregloReal[j-1]+arregloReal[j]+arregloReal[j+1])/3;
      sem_post(&semA);
      wait(&semB);
      for(int j = 0; j<50; j++)
      	arregloReal[j+50] = arregloAuxiliar[j];
      sem_post(&semA);
      wait(&semB);  
    }
}

int main(){
	sem_init(&semA,0,1);
  sem_init(&semB,0,1);
	int arregloReal[100] = (int*)malloc(sizeof(int)*100);
	pthread_t thread[2];
  pthread_create(&(thread[0]),NULL,(void*)CalcularA,(void*)arregloReal);
  pthread_create(&(thread[1]),NULL,(void*)CalcularB,(void*)arregloReal);
  pthread_join(thread[0],NULL);
  pthread_join(thread[1],NULL);
}

