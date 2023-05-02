#include "Barman/Barman.h"

void tireuse() {
    int shmidb, shmida;
    key_t keyb = 5, keya = 6;
    Tireuse* blonde;
    Tireuse* ambree;

    // Mise en place de la SHM
    shmidb = shmget(keyb, sizeof(Tireuse), IPC_CREAT | 0666);
    blonde = (Tireuse*) shmat(shmidb, NULL, 0);
    if (blonde == (Tireuse*) -1) printf("blonde.shmat: %s\n", strerror(errno));;

    shmida = shmget(keya, sizeof(Tireuse), IPC_CREAT | 0666);
    ambree = (Tireuse*) shmat(shmida, NULL, 0);
    if (ambree == (Tireuse*) -1) printf("ambree.shmat: %s\n", strerror(errno));;

    // Initialisation des tireuses
    blonde->qte = 0;
    ambree->qte = 0;
    strcpy(blonde->nom, "null");
    strncpy(ambree->nom, "null", 50);
    strncpy(blonde->type, "blonde", 50);
    strncpy(ambree->type, "ambree", 50);
}