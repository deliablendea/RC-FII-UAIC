#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int d = 3; d * d <= n; d += 2)
        if (n % d == 0)
            return 0;
    return 1;
}

int main() {
    pid_t pid;
    int pfd[2], nr, buffer;

    // Creare canal anonim
    if (-1 == pipe(pfd)) {
        perror("Eroare la crearea canalului anonim");
        exit(1);
    }

    // Creare proces fiu
    if (-1 == (pid = fork())) {
        perror("Eroare la crearea procesului fiu");
        exit(2);
    }

    // Tatal
    if (pid) {
        // Citire numar de la tastatura
        printf("Introduceti numarul n: "); scanf("%d", &nr);

        // Scrie in canal numarul citit
        write(pfd[1], &nr, 4);

        // Inchide capatul de scriere
        close(pfd[1]);

        // Asteapta terminarea procesului fiu
        wait(NULL);

        // Citeste din buffer raspunsul copilului si afiseaza mesajul corespunzator
        read(pfd[0], &buffer, 4);

        if (buffer == 0) printf("No.\n");
        else printf("Yes.\n");

        // Inchide capatul de citire
        close(pfd[0]);
    }

    // Fiul
    else {
        // Citeste numarul din canal si il salveaza in buffer
        read(pfd[0], &buffer, 4);

        // Inchide capatul de citire
        close(pfd[0]);

        // Transmite tatalui rezultatul verificarii primalitatii
        int rezultat = prime(buffer);
        write(pfd[1], &rezultat, 4);

        // Inchide capatul de scriere
        close(pfd[1]);
    }

    exit(0);
}