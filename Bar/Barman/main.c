#include "Barman.h"

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

        mkfifo("pipes/demande", 0666); // Pipe communication à main
        mkfifo("pipes/recu", 0666); // Pipe main à communication
        while (1) {
            //kill(getpid(), SIGSTOP); // Attend que le père donne la main
            principal(); 
        }
    }
    else if ((pidCom = fork()) == 0) {
        // Code processus Communication

        int socket = socketTCP();
        while (1) {
            //kill(getpid(), SIGSTOP); // Attend que le père donne la main
            communication(socket);
        }
    }
    else if ((pidScr = fork()) == 0) {
        // Code processus Sécurité
        //signal (SIGINT, fermeture());
        while (1) {
            kill(getpid(), SIGSTOP); // Attend que le père donne la main
        }
    }
    
    while (1) {
        // Main travail
        kill(pidMain, SIGCONT);
        sleep(1);
        //kill(pidMain, SIGSTOP);
        // Com travail
        kill(pidCom, SIGCONT);
        sleep(1);
        //kill(pidCom, SIGSTOP);
        // Sécurité travail
        kill(pidScr, SIGCONT);
        sleep(1);
        //kill(pidScr, SIGSTOP);
    }
}

int principal() {
    int fd;
    char buffer[1];
    int valeur;
    char* reponse;
    int n;

    // Ouvre pipe
    fd = open("pipes/demande", O_RDONLY);
    while(n = (read(fd, buffer, 1)) > 0) { // >0 EOF
        // Converstion char* en long
        valeur = strtol(buffer, NULL, 0);
        if(valeur == 0) return 0; // En cas d'erreurs
        // Ajout la demande dans l'ordonnanceur
        ajout(valeur);
        memset(buffer,0,sizeof(buffer));
    }
    if (n == -1) puts("erreur");
    close(fd);
    // Traite de la demande
    reponse = traiter();
    // Incrémentation de l'ordonnanceur
    devant++;
    if (devant > derriere) devant = derriere = -1;
    // Envoie la commande
    fd = open("pipes/recu", O_WRONLY);
    write(fd, reponse, 500);
    close(fd);
    return 1;
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
        char buffer[500];
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
            char buf[500];
            sprintf(buffer, "Voici ce que nous pouvons vous proposer :\n une bière %s de marque %s et une bière %s de marque %s", blonde->type, blonde->nom, ambree->type, ambree->nom);
            strncpy(buf, buffer, sizeof(buffer)); // Permet de copier la valeur et non le pointeur
            reponse = buf;
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