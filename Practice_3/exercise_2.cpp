#include <iostream>
#include <stack>
#include <string>

void ingresarTexto(std::stack<char>& pila);
void mostrarTextoInvertido(std::stack<char>& pila);

int main() {
    std::stack<char> pila;

    ingresarTexto(pila);
    mostrarTextoInvertido(pila);

    return 0;
}

void ingresarTexto(std::stack<char>& pila) {
    std::string texto;

    std::cout << "=== INVERTIR TEXTO ===" << std::endl;
    std::cout << "Ingrese una palabra o frase: ";
    std::getline(std::cin, texto);

    for (char letra : texto) {
        pila.push(letra);
    }
}

void mostrarTextoInvertido(std::stack<char>& pila) {
    std::cout << "\nTexto invertido: ";

    while (!pila.empty()) {
        std::cout << pila.top();
        pila.pop();
    }

    std::cout << std::endl;
}
