#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

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

int CreerSocket(char* addr, int port) {
    // identifiant de la machine serveur
    struct hostent *serveur_host;
    // adresse de la socket coté serveur
    static struct sockaddr_in addr_serveur;
    // taille de l'addresse socket
    socklen_t lg;
    // descripteur de la socket locale
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0); // TCP
    if (sock == -1) {
        perror("erreur socket");
        return -1;
    }
    serveur_host = gethostbyname(addr);
    if (serveur_host == NULL)
    {
        perror("erreur adresse serveur");
        return -1;
    }
    // création adresse socket destinatrice
    bzero(&addr_serveur, sizeof(struct sockaddr_in));
    addr_serveur.sin_family = AF_INET;
    addr_serveur.sin_port = htons(port);
    memcpy(&addr_serveur.sin_addr.s_addr, serveur_host->h_addr, serveur_host->h_length);
    if (connect(sock, (struct sockaddr *)&addr_serveur, sizeof(struct sockaddr_in)) == -1) {
        perror("erreur connexion serveur");
        exit(1);
    }
    return sock;
}

int traiter(int commande, int sock) {
    struct requete req;
    int taille_msg;
    char* message;
    char* resultat;
    req.type_requete = commande;
    req.taille_requete = sizeof(int);
    taille_msg = sizeof(struct requete) + sizeof(int);
    message = (char *)malloc(taille_msg);
    memcpy(message, &req, sizeof(struct requete));
    memcpy(message + sizeof(struct requete), &commande, sizeof(int));
    if (write(sock, message, taille_msg) <= 0)
    {   
        free(message);
        return -1;
    }
    if (read(sock, &resultat, sizeof(char*)) <= 0)
    {
        free(message);
        return -1;
    }
    free(message);
    //printf('%s', resultat);
    return 1;
}

int main() {
    int sock = CreerSocket("localhost", 4080);
    int commande = 0;
    struct requete demande;
    if (sock == -1) {
        perror("Connexion Socket Server\n");
        return 1;
    }

    printf("connexion établie");

        printf("Commande : \n 1 - Informations \n 2 - Blonde demi \n 3 - Blonde pinte \n 4 - Ambrée demi \n 5 - Ambrée Pinte \n");
        scanf("%d", &commande);
    traiter(commande, sock);

    close(sock);
}