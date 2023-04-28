#include "Barman.h"
#define TAILLEMAX 50

int queue[TAILLEMAX]; // Ordonnanceur FIFO
int devant = -1, derriere = -1;

int main() {
    pid_t pidCtrl, pidMain, pidCom, pidScr;
    if ((pidCtrl = fork()) == 0) {
        // Code processus Controle

        tireuse();
        controle();
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
        signal (SIGINT, fermeture());
        while (1) {
            kill(getpid(), SIGSTOP); // Attend que le père donne la main
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
    char* reponse;

    // Ouvre pipe
    fd = open("pipe", O_RDWR);
    while((n = read(fd, buffer, 1) ) > 0) { // Attente lecture
        // Converstion char* en long
        valeur = strtol(buffer, NULL, 0);
        // Ajout la demande dans l'ordonnanceur
        ajout(valeur);
        // Occupe de la demande
        reponse = traiter();
        // Incrémentation de l'ordonnanceur
        devant++;
        if (devant > derriere) devant = derriere = -1;
        // Envoie la commande
        printf("message : %s", reponse);
        write(fd, reponse, 500);
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

char* traiter() {
    if (devant == -1) puts("Queue est vide");
    else {
        int shmidb, shmida;
        key_t keyb = 5, keya = 6;
        Tireuse* blonde;
        Tireuse* ambree;
        char* reponse;

        // SHM
        shmidb = shmget(keyb, sizeof(Tireuse), IPC_CREAT | 0666);
        blonde = (Tireuse*) shmat(shmidb, NULL, 0);

        shmida = shmget(keya, sizeof(Tireuse), IPC_CREAT | 0666);
        ambree = (Tireuse*) shmat(shmida, NULL, 0);

        printf("client traité : %d\n", queue[devant]);
        // Prise en charge de la demande
        switch (queue[devant]) {
        case 1: // Informations
            sprintf(reponse, "Voici ce que nous pouvons vous proposer :\n une bière %s, %s et une bière %s, %s", blonde->type, blonde->nom, ambree->type, ambree->nom);
            break;
        case 2: // Blonde demi
            if(blonde->qte < 25) reponse = "nous avons malheuresement plus de blonde";
            else {
                sleep(2);
                blonde->qte = blonde->qte - 25;
                reponse = "blonde demi";
            }
            break;
        case 3: // Blonde pinte
            if(blonde->qte < 50) reponse = "nous avons malheuresement plus de blonde";
            else {
                sleep(4);
                blonde->qte = blonde->qte - 50;
                reponse = "blonde pinte";
            }
            break;
        case 4: // Ambree demin
            if(ambree->qte < 25) reponse = "nous avons malheuresement plus d'ambree";
            else {
                sleep(2);
                ambree->qte = ambree->qte - 25;
                reponse = "ambree demi";
            }
            break;
        case 5: // Ambree pinte
            if(ambree->qte < 50) reponse = "nous avons malheuresement plus d'ambree";
            else {
                sleep(4);
                ambree->qte = ambree->qte - 50;
                reponse = "ambree demi";
            }
            break;
        default:
            reponse = "demande inconnue";
            break;
        };
        return reponse;
    }
}