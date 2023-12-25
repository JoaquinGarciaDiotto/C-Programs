#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#define Lect 0
#define Escr 1
int pA[2], pB[2], pC[2], pDE[2],  pF[2];

void A() {
    close(pDE[Escr]);
    close(pDE[Lect]);
    close(pF[Escr]);
    close(pF[Lect]);

    close(pA[Escr]);
    close(pB[Lect]);
    close(pC[Lect]);

    char buf = ' ';

	while(1){
        read(pA[Lect], &buf, 1);
        printf("A");
        fflush(stdout);
        write(pB[Escr],&buf,1);
        write(pC[Escr],&buf,1);
        read(pA[Lect], &buf, 1);
	}
	
	exit(0);
}

void B() {
    close(pC[Lect]);
    close(pC[Escr]);
    close(pF[Escr]);
    close(pF[Lect]);

    close(pB[Escr]);
    close(pA[Lect]);
    close(pDE[Lect]);

    char buf = ' ';

	while(1){
        read(pB[Lect], &buf, 1);
        printf("B");
        fflush(stdout);
        write(pDE[Escr],&buf,1);
        write(pA[Escr],&buf,1);
        read(pB[Lect], &buf, 1);
	}
	
	exit(0);
}

void C() {
    close(pA[Escr]);
    close(pA[Lect]);
    close(pB[Escr]);
    close(pB[Lect]);
    close(pF[Escr]);
    close(pF[Lect]);

    close(pC[Escr]);
    close(pDE[Lect]);

    char buf = ' ';

	while(1){
		read(pC[Lect], &buf, 1);
        read(pC[Lect], &buf, 1);
		printf("C");
		fflush(stdout);
		write(pDE[Escr],&buf,1);
	}
	
	exit(0);
}

void D() {
    close(pB[Escr]);
    close(pB[Lect]);
    close(pC[Lect]);
    close(pC[Escr]);

    close(pDE[Escr]);
    close(pF[Lect]);
    close(pA[Lect]);

    char buf = ' ';

	while(1){
		read(pDE[Lect], &buf, 1);
		printf("D");
        fflush(stdout);
        write(pF[Escr],&buf,1);
		write(pA[Escr],&buf,1);
	}
	
	exit(0);
}

void E() {
    close(pB[Escr]);
    close(pB[Lect]);
    close(pC[Lect]);
    close(pC[Escr]);

    close(pDE[Escr]);
    close(pF[Lect]);
    close(pA[Lect]);

    char buf = ' ';

	while(1){
		read(pDE[Lect], &buf, 1);
        printf("E");
        fflush(stdout);
		write(pA[Escr],&buf,1);
        write(pF[Escr],&buf,1);
	}
	
	exit(0);
}

void F() {
    close(pB[Escr]);
    close(pB[Lect]);
    close(pC[Lect]);
    close(pC[Escr]);
    close(pDE[Escr]);
    close(pDE[Lect]);

    close(pF[Escr]);
    close(pA[Lect]);

    char buf = ' ';

	while(1){
		read(pF[Lect], &buf, 1);
        read(pF[Lect], &buf, 1);
		printf("F");
		fflush(stdout);
		write(pA[Escr],&buf,1);
    }
	
	exit(0);
}

int main() {

    //Creacion de las pipes
  	
  	if(pipe(pA)<0){
        perror("pipe(pA)");
        exit(1);
    }
  	if(pipe(pB)<0){
        perror("pipe(pB)");
        exit(1);
    }
  	if(pipe(pC)<0){
        perror("pipe(pC)");
        exit(1);
    }
    if(pipe(pDE)<0){
        perror("pipe(pDE)");
        exit(1);
    }
    if(pipe(pF)<0){
        perror("pipe(pF)");
        exit(1);
    }

    int buf = ' ';

    // CREAMOS PROCESOS
	int pid;

    pid = fork();

    if (pid == 0) {
        A();
    } else if (pid == -1) { 
		perror("fork()");
		exit(1);
    } else {
        int pid2 = fork();
        if (pid2 == 0) {
            B();
        } else if (pid2 == -1) { 
			perror("fork()");
			exit(1);
		} else {
            int pid3 = fork();
            if (pid3 == 0) {
                C();
            } else if (pid3 == -1) { 
				perror("fork()");
				exit(1);
			}else {
                int pid4 = fork();
                if (pid4 == 0) {
                    D();
                } else if (pid4 == -1) { 
					perror("fork()");
					exit(1);
				} else {
                    int pid5 = fork();
                    if (pid5 == 0) {
                        E();
                    } else if (pid5 == -1) { 
						perror("fork()");
						exit(1);
					} else {
                        int pid6 = fork();
                        if (pid6 == 0) {
                            F();
                        } else if (pid6 == -1) { 
							perror("fork()");
							exit(1);
						}
                    }
                }
            }
        }
    }
    
    write(pA[Escr],&buf,1);
    wait(NULL);

	return(0);
}

