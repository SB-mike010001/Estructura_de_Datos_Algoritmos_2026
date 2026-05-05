#include <stdio.h>   // Entrada y salida estándar
#include <stdlib.h>  // Utilidades generales
#include <pthread.h> // Manejo de hilos y sincronización

#define NUM_TRABAJADORES 4
#define NUM_TAREAS 8

int tareas[NUM_TAREAS];
int indice_tarea = 0;

pthread_mutex_t mutex;

/*
 * Asigna tareas de forma segura a los trabajadores.
 */
int obtener_tarea() {
    int tarea = -1;

    pthread_mutex_lock(&mutex);

    if (indice_tarea < NUM_TAREAS) {
        tarea = tareas[indice_tarea];
        indice_tarea++;
    }

    pthread_mutex_unlock(&mutex);

    return tarea;
}

/*
 * Cada trabajador solicita y procesa tareas hasta que no queden.
 */
void* trabajador(void* arg) {
    int id = *(int*)arg;

    while (1) {
        int tarea = obtener_tarea();

        if (tarea == -1) break;

        int resultado = tarea * tarea;

        printf("Trabajador %d procesa %d -> %d\n", id, tarea, resultado);
    }

    return NULL;
}

/*
 * Modelo Maestro-Trabajador:
 * El maestro distribuye tareas a múltiples trabajadores,
 * quienes las procesan de forma concurrente hasta completarlas.
 */
int main() {
    pthread_t hilos[NUM_TRABAJADORES];
    int ids[NUM_TRABAJADORES];

    printf("=== MODELO MAESTRO-TRABAJADOR ===\n");

    for (int i = 0; i < NUM_TAREAS; i++) {
        tareas[i] = i + 1;
    }

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_TRABAJADORES; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, trabajador, &ids[i]);
    }

    for (int i = 0; i < NUM_TRABAJADORES; i++) {
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Maestro: todas las tareas fueron procesadas\n");

    return EXIT_SUCCESS;
}
