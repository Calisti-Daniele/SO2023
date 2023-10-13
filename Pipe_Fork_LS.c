#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "ourhdr.h"

#define COMMAND "ls"
#define FILENAME "ls_output.txt"

int main() {


	startCounting();

	FILE *pipe = popen(COMMAND, "r");
	int written = 0, bytesToWrites, bytesRead;


	if (pipe == NULL) {
		err_sys("Errore nell'esecuzione del comando");
		return 1;
	}


    // Crea un processo figlio per eseguire il comando
    pid_t pid = fork();

    if (pid < 0) {
        err_sys("Errore nella creazione del processo");
        return 1;
    } else if (pid == 0) {  // Processo figlio

        int fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0640);
        if(fd<0)
			err_sys("Cannot open file %s", FILENAME);

        char buf[128];

		while ((bytesRead = read(fileno(pipe), buf, sizeof(buf))) > 0) {
			int bytesWritten = write(fd, buf, bytesRead);
			if (bytesWritten < 0) {
				err_sys("Errore nella scrittura nel file");
			}
		}

        exit(1);
    } else {  // Processo padre
        int status;
        wait(&status);
        stopCounting();

        /*
         * WIFEXITED(status) che controlla se il processo figlio è uscito normalmente
         *WEXITSTATUS(status) che restituisce il codice di uscita del processo figlio.
         *Se il processo figlio è uscito con successo (con un codice di uscita pari a 0)
         *
         */
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("L'output di 'ls' è stato scritto nel file '%s'\n", FILENAME);
        } else {
            printf("Errore nell'esecuzione del comando 'ls'\n");
        }

        printf("Processo eseguito in %f secondi", getRealTime());


    }

    return 0;
}
