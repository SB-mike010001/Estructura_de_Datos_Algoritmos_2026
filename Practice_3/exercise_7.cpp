#include <iostream>
#include <queue>
#include <string>
#include <limits>

void mostrarMenu();
void agregarCliente(std::queue<std::string>& cola);
void atenderCliente(std::queue<std::string>& cola);
void mostrarSiguienteCliente(const std::queue<std::string>& cola);
void mostrarCantidadClientes(const std::queue<std::string>& cola);

int main() {
    std::queue<std::string> cola;
    int opcion;

    do {
        mostrarMenu();
        std::cin >> opcion;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida." << std::endl;
            continue;
        }

        switch (opcion) {
            case 1:
                agregarCliente(cola);
                break;

            case 2:
                atenderCliente(cola);
                break;

            case 3:
                mostrarSiguienteCliente(cola);
                break;

            case 4:
                mostrarCantidadClientes(cola);
                break;

            case 5:
                std::cout << "Programa finalizado." << std::endl;
                break;

            default:
                std::cout << "Opcion invalida." << std::endl;
        }

    } while (opcion != 5);

    return 0;
}

void mostrarMenu() {
    std::cout << "\n===== BANCO =====" << std::endl;
    std::cout << "1. Agregar cliente" << std::endl;
    std::cout << "2. Atender cliente" << std::endl;
    std::cout << "3. Mostrar siguiente cliente" << std::endl;
    std::cout << "4. Mostrar cantidad de clientes" << std::endl;
    std::cout << "5. Salir" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void agregarCliente(std::queue<std::string>& cola) {
    std::string nombre;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true) {
        std::cout << "Ingrese el nombre del cliente: ";
        std::getline(std::cin, nombre);

        if (!nombre.empty()) {
            cola.push(nombre);
            break;
        }

        std::cout << "Nombre invalido." << std::endl;
    }
}

void atenderCliente(std::queue<std::string>& cola) {
    if (cola.empty()) {
        std::cout << "No hay clientes en espera." << std::endl;
        return;
    }

    std::cout << "Cliente atendido: " << cola.front() << std::endl;
    cola.pop();
}

void mostrarSiguienteCliente(const std::queue<std::string>& cola) {
    if (cola.empty()) {
        std::cout << "No hay clientes en espera." << std::endl;
    } else {
        std::cout << "Siguiente cliente: " << cola.front() << std::endl;
    }
}

void mostrarCantidadClientes(const std::queue<std::string>& cola) {
    std::cout << "Cantidad de clientes en espera: " << cola.size() << std::endl;
}
