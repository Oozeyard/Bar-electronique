#include "Barman.h"
#define MAXTAILLE 1024

void controle() {

    int shmidb, shmida, sock, reponse;
    char buffer[MAXTAILLE];
    struct sockaddr_in adresse;
    socklen_t len;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    //déclaration des cordonées du serveur
    memset(&adresse, 0, sizeof(adresse));
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(1909);
    adresse.sin_addr.s_addr = INADDR_ANY;
    
    key_t keyb = 5, keya = 6;
    Tireuse* blonde;
    Tireuse* ambree;

    shmidb = shmget(keyb, sizeof(Tireuse), IPC_CREAT | 0666);
    blonde = (Tireuse*) shmat(shmidb, NULL, 0);

    shmida = shmget(keya, sizeof(Tireuse), IPC_CREAT | 0666);
    ambree = (Tireuse*) shmat(shmida, NULL, 0);

    while(1) {
        if (ambree->qte == 0) {
            puts("ambree vide");
            sendto(sock, "1", strlen("1"),MSG_CONFIRM, (const struct sockaddr *) &adresse,sizeof(adresse));

            reponse = recvfrom(sock, (char *)buffer, MAXTAILLE,MSG_WAITALL, (struct sockaddr *) &adresse,&len);
            buffer[reponse] = '\0';
            printf("%s\n", buffer);
            ambree->qte = 500;
            strncpy(ambree->nom, buffer, 50);
            printf("nouvelle biere ambree : %s\n", ambree->nom);
            memset(buffer,0,sizeof(buffer));
        }
        if (blonde->qte == 0) {
            puts("blonde vide");
            sendto(sock, "2", strlen("2"),MSG_CONFIRM, (const struct sockaddr *) &adresse,sizeof(adresse));

            reponse = recvfrom(sock, (char *)buffer, MAXTAILLE,MSG_WAITALL, (struct sockaddr *) &adresse,&len);
            buffer[reponse] = '\0';
            printf("%s\n",buffer);
            blonde->qte = 500;
            strncpy(blonde->nom, buffer, 50);
            printf("nouvelle biere blonde : %s\n", blonde->nom);
            memset(buffer,0,sizeof(buffer));
        }
        sleep(2);
    }
    close(sock);

}
