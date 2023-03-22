#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

enum requete_t {
    INFO = 1,
    BLONDEDEMI,
    BLONDEPINTE,
    AMBREEDEMI,
    AMBREEPINTE
};

struct requete{
    enum requete_t type_requete;
    int taille_requete;
};

int CreerSocket (int port) {

    static struct sockaddr_in adresse;
    int sock;
    // création de la socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("erreur création socket");
        return(-1);
    }

    // liaison de la socket sur le port local

    bzero((char *) &adresse, sizeof(adresse)); // Permet allouer la mémoire
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(port);
    adresse.sin_addr.s_addr=htonl(INADDR_ANY);

    if(bind(sock, (struct sockaddr*)&adresse, sizeof(adresse)) == -1 ) {
        perror("erreur bind");
        return(-1);
    }

    return(sock);
}

void *Handler(void *arg) {
    printf("activé");
    struct requete req;
    // récupère la socket service
    int sock = *(int*) arg;
    int nb;
    if (read(sock, &req, sizeof(struct requete)) <= 0 ){
        perror("Erreur lecture");
        return 0;
    }
    printf("%d", req.type_requete);
    switch (req.type_requete)
    {
    case INFO:
    printf("info");
        break;
    case BLONDEDEMI:
        break;
    case BLONDEPINTE:
        break;
    case AMBREEDEMI:
        break;
    case AMBREEPINTE:
        break;
    default:
        perror("Erreur handler");
        break;
    }
}

int main() {

    int socket_ecoute, socket_service;
    static struct sockaddr_in addr_client;
    int lg;
    pthread_t thread_id;

    socket_ecoute = CreerSocket(4080);
    if (socket_ecoute == -1) {
        perror("creation socket service");
        exit(1);
    }

    if (listen(socket_ecoute, 10) == -1) {
        perror("listen");
        exit(-1);
    }

    printf("Connexion effectué \n");
    while (1) {
        lg = sizeof(struct sockaddr_in);
        printf("Attend une connexion \n");
        // Attend une connection dans la socket puis crée un thread pour gérer la connection
        socket_service = accept(socket_ecoute, (struct sockaddr*)&addr_client, &lg);
        printf("thread ");
        if(pthread_create( &thread_id , NULL ,  Handler , (void*) &socket_service) < 0)
        {
            perror("erreur thread");
            return 1;
        }
    }
}