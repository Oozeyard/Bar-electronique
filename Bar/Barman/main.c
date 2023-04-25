#include "Barman.h"
#define TAILLEMAX 50

int queue[TAILLEMAX]; // Ordonnanceur FIFO
int devant = -1, derriere = -1;

int main() {
    pid_t pidCtrl, pidMain, pidCom, pidScr;
    if ((pidCtrl = fork()) == 0) {
        tireuse();
        controle();
        // Code processus Controle
    }
    else if ((pidMain = fork()) == 0) {
        // Code processus Main

        mkfifo("pipe", 0666);
        while (1) {
            kill(getpid(), SIGSTOP); // Attend que le père donne la main
            principal();
        }
    }
    else if ((pidCom = fork()) == 0) {
        // Code processus Communication

        int socket = socketTCP();
        while (1) {
            kill(getpid(), SIGSTOP); // Attend que le père donne la main
            communication(socket);
        }
    }
    else if ((pidScr = fork()) == 0) {
        // Code processus Sécurité

        while (1) {
            kill(getpid(), SIGSTOP); // Attend que le père donne la main
            securite();
        }
    }
    
    while (1) {
        kill(pidMain, SIGCONT); // Réveille Main
        sleep(1);
        kill(pidCom, SIGCONT);
        sleep(1);
        kill(pidScr, SIGCONT);
        sleep(1);
    }
    
}

int principal() {
    int fd;
    char buffer[10];
    int valeur;
    int n;

    fd = open("pipe", O_RDONLY);
    while((n = read(fd, buffer, 1) ) > 0) {
        printf("buffer = %s\n", buffer);
        valeur = strtol(buffer, NULL, 0);
        printf("valeur %d\n", valeur);
        ajout(valeur);
        traiter();
    }
}

void ajout(int valeur) {
    if (derriere == TAILLEMAX - 1) puts("Ordonnanceur remplis");
    else {
        if (devant == -1) devant = 0;
        derriere++;
        queue[derriere] = valeur;
        printf("ajout client %d\n", valeur);
    }
}

void traiter() {
    if (devant == -1) puts("Queue est vide");
    else {
        printf("client traité : %d\n", queue[devant]);
        devant++;
        if (devant > derriere) devant = derriere = -1;
    }
}