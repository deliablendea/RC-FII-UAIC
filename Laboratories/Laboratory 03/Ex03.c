#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int main() {
    int sockp[2], child;
    char buffer[1024], MSG[1024] = "Retele - ";

    // Creare socketpair
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockp) < 0) { 
        perror("Eroare creare socketpair"); 
        exit(1); 
    }

    // Creare proces fiu
    if (-1 == (child = fork())) {
        perror("Eroare la crearea procesului fiu");
        exit(2);
    }

    // Tatal
    if (child) {
        // Inchide socket-ul copilului
        close(sockp[0]);

        // Scriere sir
        if (write(sockp[1], MSG, sizeof(MSG)) < 0) {
            perror("[Parinte] Eroare la scrierea sirului");
            exit(3);
        }

        // Citire buffer
        if (read(sockp[1], buffer, sizeof(buffer)) < 0) {
            perror("[Parinte] Eroare la citirea din buffer");
            exit(4);
        }

        // Afisare
        printf("Sirul concatenat este: %s\n", buffer);

        // Inchide socket-ul parintelui
        close(sockp[1]);
    }

    // Fiul
    else {
        // Inchide socket-ul parintelui
        close(sockp[1]);

        // Citire buffer
        if (read(sockp[0], buffer, sizeof(buffer)) < 0) {
            perror("[Copil] Eroare la citirea din buffer");
            exit(5);
        }

        // Concatenare
        strcat(buffer, "Laboratorul 3");

        // Scriere concatenare in buffer
        if (write(sockp[0], buffer, sizeof(buffer)) < 0) {
            perror("[Copil] Eroare la scrierea in buffer");
            exit(6);
        }

        // Inchide socket-ul copilului
        close(sockp[0]);
    }

    exit(0);
}