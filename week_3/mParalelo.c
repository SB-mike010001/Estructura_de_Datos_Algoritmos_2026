#include <stdio.h>
#include <stdlib.h>  // Utilidades generales
#include <pthread.h> // Manejo de hilos (ejecución paralela)

#define TAM 1000
#define NUM_HILOS 4

int arreglo[TAM];
int suma_parcial[NUM_HILOS];

/*
 * Estructura para dividir el trabajo entre hilos
 */
typedef struct {
    int id;
    int inicio;
    int fin;
} DatosHilo;

/*
 * Cada hilo procesa una parte del arreglo de forma independiente.
 */
void* suma_paralela(void* arg) {
    DatosHilo* datos = (DatosHilo*) arg;
    int suma = 0;

    for (int i = datos->inicio; i < datos->fin; i++) {
        suma += arreglo[i];
    }

    suma_parcial[datos->id] = suma;

    pthread_exit(NULL);
}

/*
 * Modelo paralelo:
 * El problema se divide en partes independientes que se ejecutan
 * simultáneamente en varios hilos y luego se combinan.
 */
int main() {
    pthread_t hilos[NUM_HILOS];
    DatosHilo datos[NUM_HILOS];

    int tam_bloque = TAM / NUM_HILOS;
    int suma_total = 0;

    printf("=== MODELO PARALELO ===\n");

    for (int i = 0; i < TAM; i++) {
        arreglo[i] = 1;
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        datos[i].id = i;
        datos[i].inicio = i * tam_bloque;
        datos[i].fin = (i == NUM_HILOS - 1) ? TAM : (i + 1) * tam_bloque;

        if (pthread_create(&hilos[i], NULL, suma_paralela, &datos[i]) != 0) {
            perror("Error al crear hilo");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        suma_total += suma_parcial[i];
    }

    printf("Suma total: %d\n", suma_total);

    return EXIT_SUCCESS;
}
