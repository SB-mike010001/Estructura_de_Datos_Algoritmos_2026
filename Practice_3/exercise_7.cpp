#include <iostream>
#include <queue>
#include <string>
#include <limits>

using namespace std;

void mostrarMenu();
void agregarCliente(queue<string>& cola);
void atenderCliente(queue<string>& cola);
void mostrarSiguienteCliente(const queue<string>& cola);
void mostrarCantidadClientes(const queue<string>& cola);

int main() {
    queue<string> cola;
    int opcion;

    do {
        mostrarMenu();
        cin >> opcion;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada invalida." << endl;
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
                cout << "Programa finalizado." << endl;
                break;

            default:
                cout << "Opcion invalida." << endl;
        }

    } while (opcion != 5);

    return 0;
}

void mostrarMenu() {
    cout << "\n===== BANCO =====" << endl;
    cout << "1. Agregar cliente" << endl;
    cout << "2. Atender cliente" << endl;
    cout << "3. Mostrar siguiente cliente" << endl;
    cout << "4. Mostrar cantidad de clientes" << endl;
    cout << "5. Salir" << endl;
    cout << "Seleccione una opcion: ";
}

void agregarCliente(queue<string>& cola) {
    string nombre;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    while (true) {
        cout << "Ingrese el nombre del cliente: ";
        getline(cin, nombre);

        if (!nombre.empty()) {
            cola.push(nombre);
            break;
        }

        cout << "Nombre invalido." << endl;
    }
}

void atenderCliente(queue<string>& cola) {
    if (cola.empty()) {
        cout << "No hay clientes en espera." << endl;
        return;
    }

    cout << "Cliente atendido: " << cola.front() << endl;
    cola.pop();
}

void mostrarSiguienteCliente(const queue<string>& cola) {
    if (cola.empty()) {
        cout << "No hay clientes en espera." << endl;
    } else {
        cout << "Siguiente cliente: " << cola.front() << endl;
    }
}

void mostrarCantidadClientes(const queue<string>& cola) {
    cout << "Cantidad de clientes en espera: " << cola.size() << endl;
}
