#include "Barman.h"

int main() {
    printf("programme : %d\n", getpid());
    pid_t pidCtrl, pidMain, pidCom, pidScr;
    if ((pidCtrl = fork()) == 0) {
        printf("Controle : %d\n", getpid());
    // Code processus Controle
    }
    else if ((pidMain = fork()) == 0) {
        // Code processus Main
        printf("Main : %d\n", getpid());
        while (1) {
            kill(getpid(), SIGSTOP); // Attend que le père donne la main
            principal();
        }
    }
    else if ((pidCom = fork()) == 0) {
        // Code processus Communication
        printf("Communication : %d\n", getpid());
        //int socket = socketTCP();
        while (1) {
            kill(getpid(), SIGSTOP); // Attend que le père donne la main
            //communication(socket);
        }
    }
    else if ((pidScr = fork()) == 0) {
        // Code processus Sécurité
        printf("Securite : %d\n", getpid());
        while (1) {
            kill(getpid(), SIGSTOP); // Attend que le père donne la main
            securite();
        }
    }
    
    while (1) {
        kill(pidMain, SIGCONT); // Réveille Main
        sleep(1);
        kill(pidMain, SIGSTOP); // Arrête Main
        kill(pidCom, SIGCONT);
        sleep(1);
        kill(pidCom, SIGSTOP);
        kill(pidScr, SIGCONT);
        sleep(1);
        kill(pidScr, SIGSTOP);
    }
    
}

int principal() {
    printf("oui\n");
}