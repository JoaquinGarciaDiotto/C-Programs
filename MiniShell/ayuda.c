#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char * args[]){
    if(argc!=1){
        printf("La entrada ingresada no es valida, escriba unicamente 'ayuda'\n");
        exit(0);
    }
    printf("Comandos que se pueden utilizar:\n");
    printf("- crear_directorio <Ruta>: Crea un directorio en la ruta especificada.\n");
    printf("    Ej: 'crear_directorio /home/nombreDirectorio'\n");
    printf("- eliminar_directorio <Ruta>: Elimina el directorio vacio indicado en la ruta especificada\n");
    printf("    Ej: 'eliminar_directorio /home/nombreDirectorio'\n");
    printf("- crear_archivo <Ruta>: Crea el archivo indicado en la ruta especificada\n");
    printf("    Ej: 'crear_archivo /home/nombreDirectorio/nombreArchivo'\n");
    printf("- listar_contenido_directorio <Ruta>: Lista el contenido del directorio en la ruta especificada\n");
    printf("    Ej: 'listar_contenido_directorio /home/nombreDirectorio'\n");
    printf("- mostrar_contenido_archivo <Ruta>: Muestra el contenido del archivo indicado en la ruta especificada\n");
    printf("    Ej: 'mostrar_contenido_archivo /home/nombreDirectorio/nombreArchivo'\n");
    printf("- modificar_permisos <Ruta> <Permisos>: Modifica los permisos del archivo indicado en la ruta especificada\n");
    printf("     Ej: 'modificar_permisos /home/nombreDirectorio/nombreArchivo 0777'\n");
    printf("- ayuda: Muestra ayuda de los comandos\n");
    printf("- finalizar: Finaliza la ejecucion del programa\n");
    return 0;
}