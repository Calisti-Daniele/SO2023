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

int lsWithPipe(){
	int fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0640);
	int written = 0, bytesToWrites, bytesRead;

	if(fd<0)
		err_sys("Cannot open file %s", FILENAME);

	// Crea un pipe per catturare l'output del comando
	FILE *pipe = popen(COMMAND, "r");

	if (pipe == NULL) {
		err_sys("Errore nell'esecuzione del comando");
		return 1;
	}

	char buf[128];

	while ((bytesRead = read(fileno(pipe), buf, sizeof(buf))) > 0) {
		int bytesWritten = write(fd, buf, bytesRead);
		if (bytesWritten < 0) {
			err_sys("Errore nella scrittura nel file");
		}
	}

	// Chiude il file di output e il pipe
	close(fd);
	pclose(pipe);
}

int main() {

	startCounting();
	lsWithPipe();
	stopCounting();

    printf("L'output di 'ls' è stato scritto nel file '%s' in %f secondi.\n", FILENAME, getRealTime());

    return 0;
}
