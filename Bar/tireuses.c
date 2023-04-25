#include "Barman/Barman.h"

int tireuse() {
    int shmidb, shmida;
    key_t keyb = 5, keya = 6;
    Tireuse* blonde;
    Tireuse* ambree;

    blonde->type = 1;
    blonde->nom = "vide";
    blonde->qte = 0;
    ambree->type = 2;
    ambree->nom = "vide";
    ambree->qte = 0;

    shmidb = shmget(keyb, sizeof(Tireuse), IPC_CREAT | 0666);
    blonde = (Tireuse*) shmat(shmidb, NULL, 0);

    shmida = shmget(keya, sizeof(Tireuse), IPC_CREAT | 0666);
    ambree = (Tireuse*) shmat(shmida, NULL, 0);

    return 0;
}

tirerblonde();

tirerambree();