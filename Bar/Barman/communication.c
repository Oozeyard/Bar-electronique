#include "Barman.h"

void *Handler(void *arg) {
    puts("serveur s'occupe du client");
    // récupère la socket service
    int sock = *(int*) arg;
    char *message;
    char reponse[50];
    char* demande;
    int fd;

    message = "Bienvenue dans le bar, que puis-je vous servir ?\n 1 - Informations \n 2 - Blonde demi \n 3 - Blonde pinte \n 4 - Ambrée demi \n 5 - Ambrée Pinte \n";
    write(sock , message , strlen(message));

    read(sock, reponse, 50);
    printf("%s\n", reponse);

    memset(reponse,0,sizeof(reponse));
   /* if (strcmp(reponse, "1") || strcmp(reponse, "Informations")) demande = "1";
    else if (strcmp(reponse, "2") || strcmp(reponse, "Blonde demi")) demande = "2";
    else if (strcmp(reponse, "3") || strcmp(reponse, "Blonde pinte")) demande = "3";
    else if (strcmp(reponse, "4") || strcmp(reponse, "Ambrée demi")) demande = "4";
    else if (strcmp(reponse, "5") || strcmp(reponse, "Ambrée Pinte")) demande = "5";
    else puts("erreur");
    printf("%s\n", demande); */
    
    fd = open("pipe", O_RDWR);
    write(fd, reponse, 1);

    while((read(fd, reponse, 1)) > 0) {
        printf("reponse : %s\n", reponse);
        write(sock, reponse, strlen(reponse));
    }

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