#include <iostream>
#include <stack>
#include <string>
#include <limits>

void mostrarMenu();
void visitarPagina(std::stack<std::string>& historial);
void retroceder(std::stack<std::string>& historial);
void mostrarPaginaActual(const std::stack<std::string>& historial);

int main() {
    std::stack<std::string> historial;
    int opcion;

    do {
        mostrarMenu();
        std::cin >> opcion;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida. Ingrese un numero del menu.\n";
            continue;
        }

        switch (opcion) {
            case 1:
                visitarPagina(historial);
                break;

            case 2:
                retroceder(historial);
                break;

            case 3:
                mostrarPaginaActual(historial);
                break;

            case 4:
                std::cout << "Programa finalizado." << std::endl;
                break;

            default:
                std::cout << "Opcion invalida." << std::endl;
        }

    } while (opcion != 4);

    return 0;
}

void mostrarMenu() {
    std::cout << "\n===== HISTORIAL DE NAVEGACION =====" << std::endl;
    std::cout << "1. Visitar pagina" << std::endl;
    std::cout << "2. Retroceder" << std::endl;
    std::cout << "3. Mostrar pagina actual" << std::endl;
    std::cout << "4. Salir" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void visitarPagina(std::stack<std::string>& historial) {
    std::string pagina;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, pagina);

    historial.push(pagina);

    std::cout << "Pagina visitada correctamente." << std::endl;
}

void retroceder(std::stack<std::string>& historial) {
    if (historial.empty()) {
        std::cout << "No hay paginas en el historial." << std::endl;
        return;
    }

    std::cout << "Retrocediendo desde: " << historial.top() << std::endl;
    historial.pop();

    if (historial.empty())
        std::cout << "No hay una pagina actual." << std::endl;
    else
        std::cout << "Pagina actual: " << historial.top() << std::endl;
}

void mostrarPaginaActual(const std::stack<std::string>& historial) {
    if (historial.empty())
        std::cout << "No hay paginas visitadas." << std::endl;
    else
        std::cout << "Pagina actual: " << historial.top() << std::endl;
}
