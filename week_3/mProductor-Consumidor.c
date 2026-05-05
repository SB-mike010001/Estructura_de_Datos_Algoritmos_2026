#include <stdio.h>   
#include <stdlib.h>   // Utilidades generales
#include <pthread.h>  // Manejo de hilos
#include <semaphore.h> // Semáforos para sincronización

#define TAM_BUFFER 5
#define NUM_ITEMS 10

int buffer[TAM_BUFFER];
int in = 0;
int out = 0;

sem_t empty;
sem_t full;

pthread_mutex_t mutex;

/*
 * El productor genera datos y los coloca en el buffer compartido.
 */
void* productor(void* arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {

        int item = i;

        sem_wait(&empty);

        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("Productor produce: %d\n", item);

        in = (in + 1) % TAM_BUFFER;

        pthread_mutex_unlock(&mutex);

        sem_post(&full);
    }

    return NULL;
}

/*
 * El consumidor toma datos del buffer compartido.
 */
void* consumidor(void* arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {

        sem_wait(&full);

        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        printf("Consumidor consume: %d\n", item);

        out = (out + 1) % TAM_BUFFER;

        pthread_mutex_unlock(&mutex);

        sem_post(&empty);
    }

    return NULL;
}

/*
 * Modelo Productor-Consumidor:
 * Un hilo produce datos y otro los consume usando un buffer compartido.
 * Se sincronizan mediante semáforos y mutex para evitar conflictos.
 */
int main() {
    pthread_t prod, cons;

    printf("=== PRODUCTOR-CONSUMIDOR ===\n");

    sem_init(&empty, 0, TAM_BUFFER);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod, NULL, productor, NULL);
    pthread_create(&cons, NULL, consumidor, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}
