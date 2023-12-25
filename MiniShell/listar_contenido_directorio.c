#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char * args[]){
    if(argc!=2){
        printf("La entrada ingresada no es valida, escriba 'ayuda'\n");
        exit(0);
    }
    DIR *d = opendir(args[1]);      //Accedemos al directorio
    struct dirent *dir;
    if(d!=NULL){
        dir = readdir(d);       //Leer entrada del directorio
        while(dir != NULL){
            printf("%s\n",dir->d_name);
            dir = readdir(d);
        }
        closedir(d);
    }
    else {
        printf("Error al listar el contenido del directorio %s\n",args[1]);
        perror("opendir");
    }
    return 0;
}