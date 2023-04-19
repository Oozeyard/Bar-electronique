#include "Barman.h"

int main() {
    pid_t pidCtrl, pidMain, pidCom, pidScr;
    if ((pidCtrl = fork()) == 0) {
    // Code processus Controle
    }
    if ((pidMain = fork()) == 0) {
        // Code processus Main
        while (1) {
            kill(getpid(), SIGSTOP); // Attend que le père donne la main
            puts("Bonjour je suis pid Main et je travaille");
        }
    }
    if ((pidCom = fork()) == 0) {
        // Code processus Communication
        while (1) {
            kill(getpid(), SIGSTOP); // Attend que le père donne la main
            puts("Bonjour je suis pid Com et je travaille");
        }
    }
    if ((pidScr = fork()) == 0) {
        // Code processus Sécurité
        while (1) {
            kill(getpid(), SIGSTOP); // Attend que le père donne la main
            puts("Bonjour je suis pid Scr et je travaille");
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
