#include <iostream>
#include <queue>
#include <string>
#include <limits>

void registrarClientes(std::queue<std::string>& cola);
void atenderCliente(std::queue<std::string>& cola);
void mostrarClientesPendientes(std::queue<std::string> cola);

int main() {
    std::queue<std::string> cola;

    registrarClientes(cola);
    atenderCliente(cola);
    mostrarClientesPendientes(cola);

    return 0;
}

void registrarClientes(std::queue<std::string>& cola) {
    int cantidad;

    std::cout << "Cantidad de clientes a registrar: ";
    std::cin >> cantidad;

    if (std::cin.fail() || cantidad <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Cantidad invalida." << std::endl;
        return;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string nombre;

    for (int i = 1; i <= cantidad; i++) {
        while (true) {
            std::cout << "Ingrese el nombre del cliente " << i << ": ";
            std::getline(std::cin, nombre);

            if (!nombre.empty()) {
                cola.push(nombre);
                break;
            }

            std::cout << "Nombre invalido. Intente nuevamente." << std::endl;
        }
    }
}

void atenderCliente(std::queue<std::string>& cola) {
    if (cola.empty()) {
        std::cout << "\nNo hay clientes para atender." << std::endl;
        return;
    }

    std::cout << "\nCliente atendido: " << cola.front() << std::endl;
    cola.pop();
}

void mostrarClientesPendientes(std::queue<std::string> cola) {
    if (cola.empty()) {
        std::cout << "No hay clientes pendientes." << std::endl;
        return;
    }

    std::cout << "\nClientes pendientes:" << std::endl;

    while (!cola.empty()) {
        std::cout << "- " << cola.front() << std::endl;
        cola.pop();
    }
}
