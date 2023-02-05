#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

// Dichiarazione del semaforo per la stanza
sem_t stanza;

// Dichiarazione dei semafori per le posate
sem_t posata[5];

// Funzione per i filosofi
void * filosofo(void *);

// Funzione per mangiare
void mangiare(int);

int main() {
    int i, filosofi[5];
    pthread_t thread[5];
    
    // Inizializzazione del semaforo per la stanza a 4 posti disponibili
    sem_init(&stanza, 0, 4);
    
    // Inizializzazione dei semafori per le posate a 1 posata disponibile per ogni filosofo
    for (i = 0; i < 5; i++) {
        sem_init(&posata[i], 0, 1);
    }
    
    // Creazione dei thread per i 5 filosofi
    for (i = 0; i < 5; i++) {
        filosofi[i] = i;
        pthread_create(&thread[i], NULL, filosofo, (void *) &filosofi[i]);
    }
    
    // Attesa per la terminazione dei thread dei filosofi
    for (i = 0; i < 5; i++) {
        pthread_join(thread[i], NULL);
    }
}

void * filosofo(void * num) {
    int id = *(int *)num;
    
    // Attesa per entrare nella stanza
    sem_wait(&stanza);
    printf("\nIl filosofo %d è entrato nella stanza", id);
    
    // Attesa per prendere le posate
    sem_wait(&posata[id]);
    sem_wait(&posata[(id + 1) % 5]);
    
    // Filosofo che mangia
    mangiare(id);
    sleep(2);
    printf("\nIl filosofo %d ha finito di mangiare\n", id);
    
    // Rilascio delle posate
    sem_post(&posata[(id + 1) % 5]);
    sem_post(&posata[id]);
    
    // Rilascio della stanza
    sem_post(&stanza);
}

void mangiare(int id) {
    printf("\nIl filosofo %d sta mangiando", id);
}
