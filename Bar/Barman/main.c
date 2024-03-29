#include "Barman.h"

int queue[TAILLEMAX]; // Ordonnanceur FIFO
int devant = -1, derriere = -1;

static void fermeture(int sig) {
    printf("fermeture %d\n", getpid());
    exit(1);
}

int main() {
    mkfifo("pipes/demande", 0666); // Pipe communication à main
    mkfifo("pipes/recu", 0666); // Pipe main à communication
    pid_t pidCtrl, pidMain, pidCom;
    signal(SIGINT,fermeture);

    if ((pidCtrl = fork()) == 0) {
        // Code processus Controle
        tireuse(); // Initialisation de la SHM
        while (1) {
            controle();
        }
    }
    else if ((pidMain = fork()) == 0) {
        // Code processus Main

        while (1) {
            principal(); 
        }
    }
    else if ((pidCom = fork()) == 0) {
        // Code processus Communication

        int socket = socketTCP(); // Création de la socket TCP
        while (1) {
            communication(socket);
        }
    }

    while (1) {
        //Main travail
        kill(pidMain, SIGCONT);
        usleep(300000);
        kill(pidMain, SIGSTOP);
        // Com travail
        kill(pidCom, SIGCONT);
        usleep(300000);
        kill(pidCom, SIGSTOP);
        // Controle travail
        kill(pidCtrl, SIGCONT);
        usleep(300000);
        kill(pidCtrl, SIGSTOP);
    }
}

int principal() {
    int fdd, fdr;
    char buffer[2];
    int valeur;
    char* reponse;

    // Ouvre pipe
    fdd = open("pipes/demande", O_RDONLY);
    fdr = open("pipes/recu", O_WRONLY);
    if (fdr == -1 || fdd == -1) puts("erreur pipes");
    if((read(fdd, buffer, 2)) > 0) { // Lecture du pipe
        // Converstion char* en long
        valeur = strtol(buffer, NULL, 0); // Si buffer est contient un char alors il retourne 0
        // Ajout la demande dans l'ordonnanceur
        ajout(valeur);
        memset(buffer,0,sizeof(buffer));
        reponse = traiter();
        // Incrémentation de l'ordonnanceur
        devant++;
        if (devant > derriere) devant = derriere = -1;
        // Envoie la commande
        printf("écrit : %s\n", reponse);
        write(fdr, reponse, 500);
    }
    close(fdd);
    close(fdr);
    return 1;
}

void ajout(int valeur) { // Ajout du client dans la queue
    if (derriere == TAILLEMAX - 1) puts("Ordonnanceur remplis");
    else {
        if (devant == -1) devant = 0;
        derriere++;
        queue[derriere] = valeur;
        printf("ajout client %d\n", valeur);
    }
}

char* traiter() {
        int shmidb, shmida;
        key_t keyb = 5, keya = 6;
        Tireuse* blonde;
        Tireuse* ambree;
        char buffer[500];
        char buf[500]; // variable tampon
        char* reponse;

        // SHM
        shmidb = shmget(keyb, sizeof(Tireuse), IPC_CREAT | 0666);
        blonde = (Tireuse*) shmat(shmidb, NULL, 0);

        shmida = shmget(keya, sizeof(Tireuse), IPC_CREAT | 0666);
        ambree = (Tireuse*) shmat(shmida, NULL, 0);

        printf("client traité : %d\n", queue[devant]);
        printf("blonde : %d cl, ambree : %dcl\n",blonde->qte, ambree->qte);
        // Prise en charge de la demande
        switch (queue[devant]) {
        case 1: // Informations
            sprintf(buffer, "Voici ce que nous pouvons vous proposer :\n une bière %s de marque %s et une bière %s de marque %s", blonde->type, blonde->nom, ambree->type, ambree->nom);
            strncpy(buf, buffer, sizeof(buffer)); // Permet de copier la valeur et non le pointeur
            reponse = buf;
            break;
        case 2: // Blonde demi
            if(blonde->qte < 25) reponse = "nous avons malheuresement plus de blonde";
            else {
                puts("preparation blonde demi");
                sleep(2);
                blonde->qte = blonde->qte - 25;
                reponse = "blonde demi";
            }
            break;
        case 3: // Blonde pinte
            if(blonde->qte < 50) reponse = "nous avons malheuresement plus de blonde";
            else {
                puts("preparation blonde pinte");
                sleep(4);
                blonde->qte = blonde->qte - 50;
                reponse = "blonde pinte";
            }
            break;
        case 4: // Ambree demi
            if(ambree->qte < 25) reponse = "nous avons malheuresement plus d'ambree";
            else {
                puts("preparation ambree demi");
                sleep(2);
                ambree->qte = ambree->qte - 25;
                reponse = "ambree demi";
            }
            break;
        case 5: // Ambree pinte
            if(ambree->qte < 50) reponse = "nous avons malheuresement plus d'ambree";
            else {
                puts("preparation ambree pinte");
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
