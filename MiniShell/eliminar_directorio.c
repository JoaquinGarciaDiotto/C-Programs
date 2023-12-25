#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char * args[]){
    if(argc!=2){
        printf("La entrada ingresada no es valida, escriba 'ayuda'\n");
        exit(0);
    }
    if(rmdir(args[1])==-1){        //Intenta eliminar un directorio vacio
        printf("Error al eliminar el directorio %s\n",args[1]);
        perror("rmdir");
    }
    else 
        printf("Directorio %s fue eliminado\n",args[1]);
}