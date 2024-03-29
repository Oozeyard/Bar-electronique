#include "Barman.h"

void *Handler(void *arg) {
    puts("serveur s'occupe du client");
    // récupère la socket service
    Argthread args = *(Argthread*) arg;
    char message[200];
    char reponse[500];

    strncpy(message, "Bienvenue dans le bar, que puis-je vous servir ?\n 1 - Informations \n 2 - Blonde demi \n 3 - Blonde pinte \n 4 - Ambrée demi \n 5 - Ambrée Pinte \n", 200);
    write(args.sock , message , strlen(message)+1);

    read(args.sock, reponse, 2);
    printf("demande client : %s\n", reponse);

    // Envoie la demande au Main
    if (write(args.fdd, reponse, 2) < 0) printf("write fdd: %s\n", strerror(errno));

    memset(reponse,0,sizeof(reponse)); // Vide la variable reponse
    if (read(args.fdr, reponse, sizeof(reponse)) > 0) {; // Attente de la commande
        printf("reponse : %s\n", reponse);
        write(args.sock, reponse, sizeof(reponse));
    }

    close(args.sock);
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

    Argthread args;
	
    args.fdd = open("pipes/demande", O_WRONLY);
    args.fdr = open("pipes/recu", O_RDONLY);
    // Lorsqu'une connection est accepté le processus crée un thread avec la socket client en arg
    while((client_sock = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c))) {
        puts("Connection accepté");
        args.sock = client_sock;
        if( pthread_create(&thread_id, NULL,  Handler, (void*) &args) < 0) {
            perror("erreur thread");
            return 1;
        }
    }
     
    if (client_sock < 0) {
        perror("erreur client");
        return 1;
    }
    
    close(args.fdd);
    close(args.fdr);
    close(sock);
    return 0;
}
