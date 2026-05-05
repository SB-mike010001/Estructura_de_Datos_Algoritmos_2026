#include <stdio.h>  
#include <stdlib.h>  // Manejo de utilidades (EXIT_SUCCESS, EXIT_FAILURE)
#include <pthread.h> // Manejo de hilos (creación, sincronización)

#define NUM_HILOS 4
#define INCREMENTOS 100000

int contador = 0;
pthread_mutex_t mutex;

/*
 * Cada hilo incrementa el contador compartido.
 * Se usa mutex para evitar condiciones de carrera.
 */
void* incrementar_contador(void* arg) {
    for (int i = 0; i < INCREMENTOS; i++) {
        pthread_mutex_lock(&mutex);
        contador++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

/*
 * Modelo concurrente:
 * Varios hilos ejecutan simultáneamente sobre un recurso compartido.
 * Requiere sincronización para evitar inconsistencias.
 */
int main() {
    pthread_t hilos[NUM_HILOS];

    printf("=== MODELO CONCURRENTE ===\n");

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Error al inicializar mutex");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        if (pthread_create(&hilos[i], NULL, incrementar_contador, NULL) != 0) {
            perror("Error al crear hilo");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Valor final del contador: %d\n", contador);

    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}
