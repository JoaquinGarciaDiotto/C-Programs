#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char * args[]){
    if(argc!=2){
        printf("La entrada ingresada no es valida, escriba 'ayuda'\n");
        exit(0);
    }
    int fd = open(args[1],O_CREAT,0777);
    if(fd==-1){
        printf("Error al crear el archivo %s\n",args[1]);
        perror("open");
    }
    else printf("Archivo creado en: %s\n",args[1]);
    close(fd);
    return 0;
}