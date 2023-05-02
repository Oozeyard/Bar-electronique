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
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define TAILLEMAX 50 // Taille de l'ordonnanceur

typedef struct { 
    int qte;
    char type[50];
    char nom[50];
} Tireuse;

typedef struct {
    int sock;
    int fdd;
    int fdr;
} Argthread;


int principal(); // Programme main
void ajout(int valeur); // Ajout dans l'ordonnanceur
char* traiter(); // Traiter client dans l'ordonnanceur
int communication(); // Programme Communication
int socketTCP(); // Création socket TCP
static void fermeture(int sig); // Programme interruption
void controle(); // Programme controle
void tireuse(); // Programme tireuse
