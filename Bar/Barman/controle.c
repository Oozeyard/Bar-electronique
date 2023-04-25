#include "Barman.h"

int socketUDP() {
    
    int sock;
    struct sockaddr_in server;
    //Create socket
    sock = socket(AF_INET , SOCK_DGRAM , 0);
    if (sock == -1) {
        perror("erreur socket");
        return 1;
    }
    puts("Socket crée");
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(4445);
     
    //Bind
    if(bind(sock,(struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("Erreur bind");
        return 1;
    }

    return sock;
     
}

int controle() {
    int shmidb, shmida;
    key_t keyb = 5, keya = 6;
    Tireuse* blonde;
    Tireuse* ambree;

    shmidb = shmget(keyb, sizeof(Tireuse), IPC_CREAT | 0666);
    blonde = (Tireuse*) shmat(shmidb, NULL, 0);

    shmida = shmget(keya, sizeof(Tireuse), IPC_CREAT | 0666);
    ambree = (Tireuse*) shmat(shmida, NULL, 0);

    while(1) {
        if (blonde->qte == 0) {
            //puts("blonde vide");
        }
        if (ambree->qte == 0) {
            //puts("ambree vide");
        }
        sleep(2);
    }
}