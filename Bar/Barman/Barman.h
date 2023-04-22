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
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

int principal(); // Programme main
void ajout(int valeur); // Ajout dans l'ordonnanceur
void traiter(); // Traiter client dans l'ordonnanceur
int communication(); // Programme Communication
int socketTCP(); // Création socket TCP
int socketUDP(); // Création socket UDP
int securite(); // Programme Sécurite

