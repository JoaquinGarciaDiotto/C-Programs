#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


sem_t sem1, sem2, sem3, sem4, sem5, sem6;



void * s1(){
	while(1){
		sem_wait(&sem1);
		printf("S1");
		sem_post(&sem2);
        sem_post(&sem3);
        sem_post(&sem4);
		}
}

void * s2(){
	while(1){
		sem_wait(&sem2);
		printf("s2");
		sem_post(&sem5);
		}
}

void * s3(){
		while(1){
		sem_wait(&sem3);
		printf("s3");
		sem_post(&sem5);
		}
}

void * s4(){
		while(1){
		sem_wait(&sem4);
		printf("s4");
		sem_post(&sem6);
		}
}
void * s5(){
		while(1){
		sem_wait(&sem5);
        sem_wait(&sem5);
		printf("s5");
		sem_post(&sem6);
		}
}
void * s6(){
		while(1){
		sem_wait(&sem6);
        sem_wait(&sem6);
		printf("s6\n");
		sem_post(&sem1);
		}
}
int main(){
	
	sem_init(&sem1,1,1);
	sem_init(&sem2,1,0);
    sem_init(&sem3,1,0);
    sem_init(&sem4,1,0);
    sem_init(&sem5,1,0);
    sem_init(&sem6,1,0);
	
	
    pthread_t thread[6];
    
   
	pthread_create(&(thread[0]),NULL,(void*)s1,NULL);
	pthread_create(&(thread[1]),NULL,(void*)s2,NULL);
	pthread_create(&(thread[2]),NULL,(void*)s3,NULL);
	pthread_create(&(thread[3]),NULL,(void*)s4,NULL);
    pthread_create(&(thread[4]),NULL,(void*)s5,NULL);
    pthread_create(&(thread[5]),NULL,(void*)s6,NULL);

	for (int i=0;i<6;i++){ 
         pthread_join(thread[i],NULL);
    }
}

