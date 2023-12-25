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


#define N 30
#define SEGSIZE sizeof (m_i)
#define KEY ((key_t)(1243))
#define delay 1

typedef struct info {
    int buffer[N];
    int cursor;
    sem_t full;
    sem_t empty;
    sem_t mutex;
}m_i;


void productor() {
  int idBuffer = shmget(KEY,SEGSIZE,0666);
  m_i * ctrl = shmat(idBuffer,NULL,0);
  if(ctrl==(m_i*)(-1)){
    perror("shmat P");
    exit(1);
  }
	srand(getpid());
  int r = 0;
  int itemActual;
	while(1) {
    sem_wait(&(ctrl->empty));
    sem_wait(&(ctrl->mutex));
    
    itemActual = ctrl->cursor;
    r = rand() % 101;
    printf("Produciendo %i\n",r);
    (ctrl->buffer)[itemActual] = r;
    ctrl->cursor = itemActual+1;

    sem_post(&(ctrl->mutex));
    sem_post(&(ctrl->full));
    //sleep(delay);
  }
}

void consumidor() {
  int idBuffer = shmget(KEY,SEGSIZE,0666);
  m_i * ctrl = shmat(idBuffer,NULL,0);
  if(ctrl==(m_i*)(-1)){
    perror("shmat C");
    exit(1);
  }
  int v;
  int itemActual;
    while(1) {
        sem_wait(&(ctrl->full));
        sem_wait(&(ctrl->mutex));
        
        itemActual = ctrl->cursor;
        itemActual -= 1;
        ctrl->cursor = itemActual;
        v = (ctrl->buffer)[itemActual];
        
        printf("Se consumio el item: %i\n",v);

        sem_post(&(ctrl->mutex));
        sem_post(&(ctrl->empty));
        sleep(delay);
    }
}


int main() {
    int idBuffer = shmget(KEY, SEGSIZE, IPC_CREAT|0666);
    if(idBuffer==-1){
        perror("shmget");
        exit(1);
    }
    m_i *ctrl = (m_i*) shmat(idBuffer,0,0);
    if(ctrl==(m_i*)(-1)){
        perror("shmat");
        exit(1);
    }
    sem_init(&(ctrl->full), 1, 0);
    sem_init(&(ctrl->empty), 1, N);
    sem_init(&(ctrl->mutex), 1, 1);
    ctrl->cursor = 0;
    /*(ctrl->buffer) = *(int*)malloc(sizeof(int)*N);
    for(int i = 0; i<N; i++)
      (ctrl->buffer)[i] = i;*/

    int pidP = fork();
    if(pidP==0)
        productor();
    else if (pidP==-1)
      perror("fork P");
    else if (pidP>0){
      int pidC = fork();
      if(pidC==0)
          consumidor();
      else if (pidC==-1)
        perror("fork C");
      else if(pidC>0){
        waitpid(pidP,NULL,0);
        waitpid(pidC,NULL,0);
        waitpid(0,NULL,0);
        wait(NULL);
      }
    }
}

