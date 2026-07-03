#include <iostream>
#include <queue>
#include <string>
#include <limits>

void registrarPersonas(std::queue<std::string>& cola);
void atenderPersonas(std::queue<std::string>& cola);

int main() {
    std::queue<std::string> cola;

    registrarPersonas(cola);
    atenderPersonas(cola);

    return 0;
}

void registrarPersonas(std::queue<std::string>& cola) {
    std::string nombre;

    std::cout << "=== REGISTRO DE PERSONAS ===" << std::endl;

    for (int i = 1; i <= 10; i++) {
        while (true) {
            std::cout << "Ingrese el nombre de la persona " << i << ": ";
            std::getline(std::cin, nombre);

            if (!nombre.empty()) {
                cola.push(nombre);
                break;
            }

            std::cout << "Nombre invalido. Intente nuevamente." << std::endl;
        }
    }
}

void atenderPersonas(std::queue<std::string>& cola) {
    std::cout << "\n=== ATENCION DE PERSONAS ===" << std::endl;

    while (!cola.empty()) {
        std::cout << "Atendiendo a: " << cola.front() << std::endl;
        cola.pop();
    }

    std::cout << "\nTodas las personas fueron atendidas." << std::endl;
}
