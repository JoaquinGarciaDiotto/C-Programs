#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char * args[]){
    if(argc!=2){
        printf("La entrada ingresada no es valida, escriba 'ayuda'\n");
        exit(0);
    }
    if(mkdir(args[1],0777)==-1){
        printf("Error al crear el directorio %s\n",args[1]);
        perror("mkdir");
    }
    else 
        printf("Directorio creado en %s\n",args[1]);
    return 0;
}