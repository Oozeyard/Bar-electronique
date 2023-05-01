#include "Barman.h"

void *Handler(void *arg) {
    puts("serveur s'occupe du client");
    // récupère la socket service
    int sock = *(int*) arg;
    char *message;
    char reponse[200];
    int fd;

    message = "Bienvenue dans le bar, que puis-je vous servir ?\n 1 - Informations \n 2 - Blonde demi \n 3 - Blonde pinte \n 4 - Ambrée demi \n 5 - Ambrée Pinte \n";
    write(sock , message , strlen(message)+1);

    read(sock, reponse, 1);
    printf("demande client : %s\n", reponse);
    
    // Envoie la demande au Main
    fd = open("pipes/demande", O_WRONLY);
    if (write(fd, reponse, 1) < 0) puts("erreur écriture");
    close(fd);

    memset(reponse,0,sizeof(reponse)); // Vide la variable reponse

    fd = open("pipes/recu", O_RDONLY);
    if (read(fd, reponse, 200) > 0 ) { // Attente de la commande
        printf("reponse : %s\n", reponse);
        write(sock, reponse, strlen(reponse));
    }
    close(fd);
    close(sock);
}

int socketTCP() {
    
    int sock;
    struct sockaddr_in server;
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
        perror("erreur socket");
        return 1;
    }
    puts("Socket crée");
     
    //TCP
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(4444);
    int option = 1;
     
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    //Bind
    if(bind(sock,(struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("Erreur bind");
        close(sock);
        exit(1);
    }

    return sock;
     
}

int communication(int sock) {

    int client_sock , c;
    struct sockaddr_in client;
     
    
    //Listen
    puts("listen");
    listen(sock , 3);
    c = sizeof(struct sockaddr_in);
	pthread_t thread_id;
	
    // Lorsqu'une connection est accepté le processus crée un thread avec la socket client en arg
    while((client_sock = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c))) {
        puts("Connection accepté");
        if( pthread_create(&thread_id, NULL,  Handler, (void*) &client_sock) < 0) {
            perror("erreur thread");
            return 1;
        }
    }
     
    if (client_sock < 0) {
        perror("erreur client");
        return 1;
    }
    
    close(sock);
    return 0;
}