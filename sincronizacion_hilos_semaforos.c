#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>

sem_t semA, semB, semDE, semC, semF;

void A() {
	while(1){
		sem_wait(&semA);
		printf("A");
        fflush(stdout);
		sem_post(&semB);
        sem_post(&semC);
        sem_wait(&semA);
	}
}

void B() {
	while(1){
        sem_wait(&semB);
		printf("B");
        fflush(stdout);
		sem_post(&semDE);
        sem_post(&semA);
        sem_wait(&semB);
	}
}

void C() {
	while(1){
		sem_wait(&semC);
        sem_wait(&semC);
		printf("C");
        fflush(stdout);
		sem_post(&semDE);
	}
}

void D() {
	while(1){
		sem_wait(&semDE);
		printf("D");
        fflush(stdout);
        sem_post(&semF);
		sem_post(&semA);
	}
}

void E() {
	while(1){
		sem_wait(&semDE);
		printf("E");
        fflush(stdout);
        sem_post(&semF);
		sem_post(&semA);
	}
}

void F() {
	while(1){
		sem_wait(&semF);
        sem_wait(&semF);
		printf("F");
        fflush(stdout);
        sem_post(&semA);
	}
}

int main() {
	sem_init(&semA, 1, 1);
	sem_init(&semB, 1, 0);
	sem_init(&semDE, 1, 0);
	sem_init(&semC, 1, 0);
	sem_init(&semF, 1, 0);
	
	pthread_t thread[6];
  
	pthread_create(&(thread[0]),NULL,(void*)A,NULL);
	pthread_create(&(thread[1]),NULL,(void*)B,NULL);
	pthread_create(&(thread[2]),NULL,(void*)C,NULL);
	pthread_create(&(thread[3]),NULL,(void*)D,NULL);
	pthread_create(&(thread[4]),NULL,(void*)E,NULL);
	pthread_create(&(thread[5]),NULL,(void*)F,NULL);
  
	for (int i=0;i<6;i++){
		pthread_join(thread[i],NULL);
	}
	
	exit(0);
}

