#include <iostream>
#include <stack>

void ingresarNumeros(std::stack<int>& pila);
void mostrarResultados(const std::stack<int>& pila);

int main() {
    std::stack<int> pila;

    ingresarNumeros(pila);
    mostrarResultados(pila);

    return 0;
}

void ingresarNumeros(std::stack<int>& pila) {
    int numero;

    std::cout << "=== INGRESO DE NUMEROS ===\n";
    std::cout << "Escriba numeros enteros (-1 para terminar).\n";

    while (true) {
        std::cout << "Numero: ";
        std::cin >> numero;

        if (numero == -1)
            break;

        pila.push(numero);
    }
}

void mostrarResultados(const std::stack<int>& pila) {
    std::cout << "\n=== RESULTADOS ===\n";

    if (pila.empty()) {
        std::cout << "La pila esta vacia.\n";
        return;
    }

    std::cout << "Cantidad de elementos: " << pila.size() << std::endl;
    std::cout << "Ultimo elemento ingresado: " << pila.top() << std::endl;
}
