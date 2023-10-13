#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "ourhdr.h"

#define FILENAME "ls_output.txt"

int main() {

	startCounting();

    // Crea un processo figlio per eseguire il comando
    pid_t pid = fork();

    if (pid < 0) {
        err_sys("Errore nella creazione del processo");
        return 1;
    } else if (pid == 0) {  // Processo figlio

        int fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0640);
        if(fd<0)
			err_sys("Cannot open file %s", FILENAME);

        //Indico che il mio output di default non è il terminale ma il file ls_output.txt
        dup2(fd, STDOUT_FILENO);


        close(fd);

        /*
         * Esegue il comando "ls" nel processo figlio utilizzando execlp().
         * Poiché STDOUT_FILENO è stato reindirizzato verso il file "ls_output.txt",
         * l'output del comando "ls" verrà scritto nel file anziché essere stampato a schermo.
         * */
        execlp("ls", "ls", (char *)NULL);
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
