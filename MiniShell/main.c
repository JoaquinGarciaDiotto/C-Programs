#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#define MAXLENGTH 200
#define MAXPARAMETROS 3

int main(){

    char * input = (char*)malloc(sizeof(char)*MAXLENGTH);
    char ** args = (char**)malloc(sizeof(char)*(MAXPARAMETROS+1)*MAXLENGTH); 
    
    while(1) {
        printf("Ingrese comando: ");
        fgets(input, MAXLENGTH, stdin);
		input[strlen(input)-1] = '\0';		//Eliminamos el caracter de nueva linea y lo reemplazamos por el de fin de linea.

        int i = 0;
        char * tempArgs = strtok(input, " ");
        while(tempArgs!=NULL && i<MAXPARAMETROS){
            args[i] = tempArgs+'\0';
            tempArgs = strtok(NULL, " ");
            i++;
        }

        if(i==0 || (i==MAXPARAMETROS && tempArgs!=NULL)){       //Determinamos si la entrada ingresada esta dentro de los parametros definidos (Max 3)
            printf("La entrada ingresada no es valida, escriba 'ayuda'\n");
        }
        else{

			if(strcmp(args[0],"finalizar")==0){
				free(input);
				free(args);
				exit(0);
			}

			int pid = fork();

			if (pid < 0) {
				perror("fork()");
				exit(1);
			} else if (pid == 0) {
				args[i] = NULL;     //Agregamos un finalizador
				if(execv(args[0],args)==-1){
					printf("La entrada ingresada no es valida, escriba 'ayuda'\n");
					perror("Execv");
				}
        	}
		}
    	wait(NULL); 
	}
        
	return 0;
}

  
