#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

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

int traiter(int sock) {
    char reponse[200];
    char message[1];

    // la connexion est établie, on attend les données envoyées par le client
    read(sock, reponse, 200);
    // affichage du message reçu
    printf("%s\n", reponse);
    scanf("%s", message);

    // écrit la demande
    write(sock , message , strlen(message)+1);
    // lit la réponse
    memset(reponse,0,sizeof(reponse)); // Vide la variable reponse
    read(sock, reponse, 200);
    printf("%s\n", reponse);

    return 1;
}

int main() {
    int sock = CreerSocket("localhost", 4444);
    if (sock == -1) {
        perror("Connexion Socket Server\n");
        return 1;
    }

    printf("connexion établie \n");
    traiter(sock);

    close(sock);
    return 1;
}
