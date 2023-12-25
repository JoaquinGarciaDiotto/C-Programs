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


#define N 30
#define SEGSIZE sizeof (struct info)
#define KEY ((key_t)(1243))
#define delay 1
#define True 1
#define False 0
#define P 0
#define C 1

struct info {
    int buffer[N];
    int cursor;
    int turno;
    int flag[2];
};

int main(int argc, char *argv[]){
    int id = shmget(KEY,SEGSIZE,IPC_CREAT | 0666);
    if(id==-1){
        perror("shmget");
        exit(1);
    }
    struct info* ctrl = (struct info*)shmat(id,NULL,0);
    if(ctrl==(struct info*)(-1)){
        perror("shmat P");
        exit(1);
    }

    int itemActual;
    int v;
    while (True)
    {
        (ctrl->flag)[C] = True;
        ctrl->turno = P;
        while((ctrl->flag)[P] && (ctrl->turno)==P){}

        itemActual = ctrl->cursor;
        itemActual -= 1;
        ctrl->cursor = itemActual;
        v = (ctrl->buffer)[itemActual];
        
        printf("Se consumio el item: %i\n",v);

        (ctrl->flag)[C] = False;

        sleep(delay);
    }
    
    return 0;
}