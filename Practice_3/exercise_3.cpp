#include <iostream>
#include <stack>
#include <string>

void cargarPila(const std::string& texto, std::stack<char>& pila);
bool esPalindromo(const std::string& texto, std::stack<char>& pila);

int main() {
    std::string texto;
    std::stack<char> pila;

    std::cout << "=== VERIFICAR PALINDROMO ===" << std::endl;
    std::cout << "Ingrese una palabra o frase: ";
    std::getline(std::cin, texto);

    cargarPila(texto, pila);

    if (esPalindromo(texto, pila))
        std::cout << "\nEl texto es un palindromo." << std::endl;
    else
        std::cout << "\nEl texto no es un palindromo." << std::endl;

    return 0;
}

void cargarPila(const std::string& texto, std::stack<char>& pila) {
    for (char letra : texto) {
        if (letra != ' ') {
            pila.push(letra);
        }
    }
}

bool esPalindromo(const std::string& texto, std::stack<char>& pila) {
    for (char letra : texto) {
        if (letra == ' ')
            continue;

        if (letra != pila.top())
            return false;

        pila.pop();
    }

    return true;
}
