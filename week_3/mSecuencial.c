#include <stdio.h>
#include <stdlib.h>

int leer_datos() {
    int dato;

    printf("Ingrese un numero: ");
    scanf("%d", &dato);

    return dato;
}

int procesar_datos(int dato) {
    return dato * dato;
}

void mostrar_resultado(int resultado) {
    printf("Resultado: %d\n", resultado);
}

int main() {
    int dato;
    int resultado;

    printf("=== MODELO SECUENCIAL ===\n");

    dato = leer_datos();
    resultado = procesar_datos(dato);
    mostrar_resultado(resultado);

    printf("Fin del programa\n");

    return EXIT_SUCCESS;
}
