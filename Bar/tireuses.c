#include "Barman/Barman.h"

int tireuse() {
    struct Tireuse *blonde;
    blonde->type = 1;
    struct Tireuse *ambree;
    ambree->type = 2;

    int keyblonde, keyambree, statusblonde, statusambree;
    keyblonde = 5;
    keyambree = 6;

    if((statusblonde = shmget(keyblonde, sizeof(struct Tireuse), IPC_CREAT|IPC_EXCL|0600))==-1) {
        printf("shm1.shmget: %s\n", sterror(errno));
        exit(1);
    }
    if((blonde =(char*) shmat(statusblonde, NULL, 0)) == (char*)-1){
        printf("shm1.shmat: %s\n", sterror(errno));
        exit(1);
    }

    if((statusambree = shmget(keyambree, sizeof(struct Tireuse), IPC_CREAT|IPC_EXCL|0600))==-1) {
        printf("shm1.shmget: %s\n", sterror(errno));
        exit(1);
    }
    if((ambree =(char*) shmat(statusambree, NULL, 0)) == (char*)-1){
        printf("shm1.shmat: %s\n", sterror(errno));
        exit(1);
    }

    return 0;
}