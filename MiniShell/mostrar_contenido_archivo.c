#include <stdio.h>
#include <stdlib.h>
int main(int argc, char * args[]){
    if(argc!=2){
        printf("La entrada ingresada no es valida, escriba 'ayuda'\n");
        exit(0);
    }
    FILE *fp = fopen(args[1],"r");
    if(fp==NULL){
        printf("Error al abrir el archivo %s\n",args[1]);
        perror("fopen()");
        exit(0);
    }
    char ch = fgetc(fp);     //Obtenemos caracteres del archivo
    while(ch != EOF){
        putchar(ch);        //Lo mostramos por pantalla
        ch = fgetc(fp);
    }
    return 0;
}