#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#ifndef SEM_R
#define SEM_R 0400
#endif

#ifndef SEM_A
#define SEM_A 0200
#endif

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void init_smph(int *sm)
{
    // Get a semaphore set with one semaphore.
    sm[0] = semget(IPC_PRIVATE, 1, SEM_R | SEM_A);
    if (sm[0] == -1) {
        fprintf(stderr, "could not obtain semaphore set\n");
        exit(-1);
    }

    // Initialize the semaphore.
    union semun val;
    val.val = 0;
    if (semctl(sm[0], 0, SETVAL, val) == -1) {
        fprintf(stderr, "could not initialize semaphore\n");
        exit(-1);
    }
}

void destroy_smph(int *sm)
{
    union semun val;
    val.val = 0; // Avoid uninitialized variable warning.
    semctl(sm[0], 0, IPC_RMID, val);
}

void post_smph(int *sm)
{
    struct sembuf op[1];
    op->sem_num = 0;
    op->sem_op = 1;
    op->sem_flg = 0;
    if (semop(sm[0], op, 1) == -1) {
        fprintf(stderr, "could not signal semaphore");
        exit(-1);
    }
}

void wait_smph(int *sm)
{
    struct sembuf op[1];
    op->sem_num = 0;
    op->sem_op = -1;
    op->sem_flg = 0;
    if (semop(sm[0], op, 1) == -1) {
        fprintf(stderr, "could not wait for semaphore");
        exit(-1);
    }
}

int sem;
int global;

void *right(void *args)
{
    wait_smph(&sem);
    global = 0;
    return args;
}

void *left(void *args)
{
    int i;
    for (i = 1; i < 400000; ++i) {
        global = i;
    }
    post_smph(&sem);
    return args;
}

int main()
{
    init_smph(&sem);

    pthread_t thr1, thr2;
    pthread_create(&thr1, NULL, right, NULL);
    pthread_create(&thr2, NULL, left, NULL);
    pthread_join(thr1, NULL);
    pthread_join(thr2, NULL);

    destroy_smph(&sem);

    //printf("%d\n", global);
    assert(global == 0);
    return 0;
}
