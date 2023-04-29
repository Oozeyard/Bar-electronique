#include "Barman/Barman.h"

int tireuse() {
    int shmidb, shmida;
    key_t keyb = 5, keya = 6;
    Tireuse* blonde;
    Tireuse* ambree;

    // Mise en place de la SHM
    shmidb = shmget(keyb, sizeof(Tireuse), IPC_CREAT | 0666);
    blonde = (Tireuse*) shmat(shmidb, NULL, 0);

    shmida = shmget(keya, sizeof(Tireuse), IPC_CREAT | 0666);
    ambree = (Tireuse*) shmat(shmida, NULL, 0);

    blonde->type = "blonde";
    blonde->nom = "vide";
    blonde->qte = 0;
    ambree->type = "ambree";
    ambree->nom = "vide";
    ambree->qte = 0;

    return 0;
}