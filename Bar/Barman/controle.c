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
     
    //TCP
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