#include "RWLock.h"
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#define SLEEP_LECTORES 1
#define SLEEP_ESCRITORES 2
#define CANT_LYE 5
#define CANT_IT 2
#define SEED 3265

RWLock lock;

void *lector(void *p_minumero) {
    int minumero = *((int *) p_minumero);
    
    printf("LECTOR %d QUIERE LEER\n", minumero);
    lock.rlock();
    printf("LECTOR %d LEYENDO\n", minumero);
    sleep(SLEEP_LECTORES);
    lock.runlock();
    printf("LECTOR %d HA LEIDO\n", minumero);
    return NULL;
}

void *escritor(void *p_minumero) {
    int minumero = *((int *) p_minumero);
    
    printf("ESCRITOR %d QUIERE ESCRBIR\n", minumero);
    lock.wlock();
    printf("ESCRITOR %d ESCRIBIENDO\n", minumero);
    sleep(SLEEP_ESCRITORES);
    lock.wunlock();
    printf("ESCRITOR %d HA ESCRITO\n", minumero);
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t lectores[CANT_IT*CANT_LYE];
    int num_lec[CANT_IT*CANT_LYE];
    pthread_t escritores[CANT_IT*CANT_LYE];
    int num_esc[CANT_IT*CANT_LYE];
    int tid;
    int it;
    srand(SEED);

    for (it = 0; it < CANT_IT; it++){
        for (tid = 0; tid < CANT_LYE; ++tid) {
            if(rand()%2 == 1) {
                pthread_create(&lectores[it*CANT_LYE + tid], NULL, lector, &(num_lec[it*CANT_LYE + tid] = it*CANT_LYE + tid));
                pthread_create(&escritores[it*CANT_LYE + tid], NULL, escritor, &(num_esc[it*CANT_LYE + tid] = it*CANT_LYE + tid));
            } else {
                pthread_create(&escritores[it*CANT_LYE + tid], NULL, escritor, &(num_esc[it*CANT_LYE + tid] = it*CANT_LYE + tid));
                pthread_create(&lectores[it*CANT_LYE + tid], NULL, lector, &(num_lec[it*CANT_LYE + tid] = it*CANT_LYE + tid));
            }
        }
    }

    for (tid = 0; tid < CANT_IT*CANT_LYE; ++tid) {
        if(num_lec[tid] == tid) pthread_join(lectores[tid], NULL);
    }

    for (tid = 0; tid < CANT_IT*CANT_LYE; ++tid) {
        if(num_esc[tid] == tid) pthread_join(escritores[tid], NULL);
    }

    return 0;
}
