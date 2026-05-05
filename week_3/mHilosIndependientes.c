#include <stdio.h>  
#include <stdlib.h>  // Utilidades generales
#include <pthread.h> // Manejo de hilos
#include <unistd.h>  // Funciones del sistema (sleep)

 /*
 * Cada hilo ejecuta una tarea independiente sin compartir datos.
 */
void* tarea_lectura(void* arg) {
    printf("[Hilo 1] Leyendo datos...\n");
    sleep(1);
    printf("[Hilo 1] Lectura completada\n");
    return NULL;
}

void* tarea_procesamiento(void* arg) {
    printf("[Hilo 2] Procesando datos...\n");
    sleep(2);
    printf("[Hilo 2] Procesamiento completado\n");
    return NULL;
}

void* tarea_escritura(void* arg) {
    printf("[Hilo 3] Escribiendo resultados...\n");
    sleep(1);
    printf("[Hilo 3] Escritura completada\n");
    return NULL;
}

/*
 * Modelo de hilos independientes:
 * Cada hilo realiza su tarea de forma autónoma,
 * sin comunicación ni sincronización entre ellos.
 */
int main() {
    pthread_t h1, h2, h3;

    printf("=== MODELO DE HILOS INDEPENDIENTES ===\n");

    pthread_create(&h1, NULL, tarea_lectura, NULL);
    pthread_create(&h2, NULL, tarea_procesamiento, NULL);
    pthread_create(&h3, NULL, tarea_escritura, NULL);

    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);

    printf("Todas las tareas independientes finalizaron\n");

    return EXIT_SUCCESS;
}
