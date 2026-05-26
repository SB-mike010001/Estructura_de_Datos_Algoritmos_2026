#include <iostream>
#include <queue>
#include <stack>
#include <ctime>

template<typename T>
std::queue<T> invertirCola(std::queue<T> cola);
template<typename T>
void imprimirCola(std::queue<T> cola);

int main() {
    std::srand(std::time(NULL));
    
    std::queue<int> cola;
    
    for (int i = 0; i < 10; i++) {
        cola.push(rand() % 100 + 1);
    }
    
    std::cout << "Cola original: ";
    imprimirCola(cola);
    
    std::queue<int> colaInvertida = invertirCola(cola);
    
    std::cout << "Cola invertida: ";
    imprimirCola(colaInvertida);
    
    return 0;
}

template<typename T>
std::queue<T> invertirCola(std::queue<T> cola) {
    std::stack<T> pila;
    
    while (!cola.empty()) {
        pila.push(cola.front());
        cola.pop();
    }
    
    std::queue<T> colaInvertida;
    while (!pila.empty()) {
        colaInvertida.push(pila.top());
        pila.pop();
    }
    
    return colaInvertida;
}

template<typename T>
void imprimirCola(std::queue<T> cola) {
    while (!cola.empty()) {
        std::cout << cola.front() << " ";
        cola.pop();
    }
    std::cout << std::endl;
}
