#include <iostream>
#include <queue>
#include <string>
#include <limits>

struct Ticket {
    int codigo;
    std::string cliente;
    int prioridad;
};

bool operator<(const Ticket& a, const Ticket& b) {
    return a.prioridad < b.prioridad;
}

void registrarTickets(std::priority_queue<Ticket>& tickets);
void mostrarOrdenAtencion(std::priority_queue<Ticket> tickets);

int main() {
    std::priority_queue<Ticket> tickets;

    registrarTickets(tickets);
    mostrarOrdenAtencion(tickets);

    return 0;
}

void registrarTickets(std::priority_queue<Ticket>& tickets) {
    int cantidad;

    std::cout << "Cantidad de tickets: ";
    std::cin >> cantidad;

    if (std::cin.fail() || cantidad <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 0; i < cantidad; i++) {
        Ticket t;

        std::cout << "\nTicket " << i + 1 << std::endl;

        std::cout << "Codigo: ";
        std::cin >> t.codigo;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Cliente: ";
        std::getline(std::cin, t.cliente);

        std::cout << "Prioridad: ";
        std::cin >> t.prioridad;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            i--;
            continue;
        }

        tickets.push(t);
    }
}

void mostrarOrdenAtencion(std::priority_queue<Ticket> tickets) {
    if (tickets.empty()) {
        std::cout << "\nNo hay tickets registrados." << std::endl;
        return;
    }

    std::cout << "\n=== Orden de atencion ===" << std::endl;

    while (!tickets.empty()) {
        Ticket t = tickets.top();

        std::cout << "Codigo: " << t.codigo
                  << " | Cliente: " << t.cliente
                  << " | Prioridad: " << t.prioridad
                  << std::endl;

        tickets.pop();
    }
}
