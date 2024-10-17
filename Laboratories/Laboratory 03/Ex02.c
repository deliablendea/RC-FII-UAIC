#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "Ex02FIFO"

int main() {
    int fd, child;

    // Creare fisier fifo cu drepturi read & write pentru proprietar
    if (-1 == mkfifo(FIFO_NAME, 0600)) {
        perror("Eroare la crearea fisierului fifo");
        exit(1);
    }

    // Creare proces fiu
    if (-1 == (child = fork())) {
        perror("Eroare la crearea procesului fiu");
        exit(2);
    }

    // Fiu
    if (child == 0) {
         // Deschide fifo pentru scriere
        if (-1 == (fd = open(FIFO_NAME, O_WRONLY))) {
            perror("Eroare la deschiderea fisierului fifo pentru scriere");
            exit(3);
        }

        // Redirectare iesire standard la fifo
        if (-1 == dup2(fd, 1)) {
            perror("Eroare la redirectarea iesirii standard in fiu");
            exit(4);
        }

        // Inchide descriptorul
        close(fd);

        // Comanda "cat prog.c"
        execlp("cat", "cat", "prog.c", NULL);

        perror("Eroare la execlp() in procesul fiu");
        exit(5);
    }

    // Parinte
    else {
        // Deschide fifo pentru scriere
        if (-1 == (fd = open(FIFO_NAME, O_RDONLY))) {
            perror("Eroare la deschiderea fisierului fifo pentru citire");
            exit(6);
        }
        
        // Redirectare intrare standard la fifo
        if (-1 == dup2(fd, 0)) {
            perror("Eroare la redirectarea intrarii standard in parinte");
            exit(7);
        }

        // Inchide descriptorul
        close(fd);

        // Comanda grep "include"
        execlp("grep", "grep", "include", NULL);

        perror("Eroare la execlp() in parinte");
        exit(8);
    }
    
    exit(0);
}