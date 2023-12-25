#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char * args[]){
    if(argc!=3){
        printf("La entrada ingresada no es valida, escriba 'ayuda'\n");
        exit(0);
    }
    if(chmod(args[1],strtol(args[2],0,8))==-1){
        printf("No se pudo modificar los permisos del archivo %s, a %s\n",args[1],args[2]);
        perror("chmod");
    }
    else {
        printf("Permisos del archivo %s modificados a %s\n",args[1],args[2]);
    }
    return 0;
}
