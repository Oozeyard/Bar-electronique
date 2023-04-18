#include "Barman.h"

void *Handler(void *arg) {
    puts("serveur s'occupe du client");
    // récupère la socket service
    int sock = *(int*) arg;
    char *message;
    char reponse[50];
    int demande;

    message = "Bienvenue dans le bar, que puis-je vous servir ?\n 1 - Informations \n 2 - Blonde demi \n 3 - Blonde pinte \n 4 - Ambrée demi \n 5 - Ambrée Pinte \n";
    write(sock , message , strlen(message));

    read(sock, reponse, 50);
    printf("%s\n", reponse);

    if (strcmp(reponse, "1") || strcmp(reponse, "Informations")) demande = 1;
    else if (strcmp(reponse, "2") || strcmp(reponse, "Blonde demi")) demande = 2;
    else if (strcmp(reponse, "3") || strcmp(reponse, "Blonde pinte")) demande = 3;
    else if (strcmp(reponse, "4") || strcmp(reponse, "Ambrée demi")) demande = 4;
    else if (strcmp(reponse, "5") || strcmp(reponse, "Ambrée Pinte")) demande = 5;
    else puts("erreur");

    close(sock);
}

int main() {

    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1) {
        perror("erreur socket");
        return 1;
    }
    puts("Socket crée");
     
    //TCP
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(4444);
     
    //Bind
    if(bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("Erreur bind");
        return 1;
    }
     
    //Listen
    puts("listen");
    listen(socket_desc , 3);
    c = sizeof(struct sockaddr_in);
	pthread_t thread_id;
	
    while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {
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
    
    close(socket_desc);
    return 0;
}