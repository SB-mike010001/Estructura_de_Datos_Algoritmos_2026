#include <iostream>
#include <string>
#include <vector>
#include <limits>

class Estudiante {
private:
    int codigo;
    std::string nombre;
    int edad;
    std::string carrera;

public:
    Estudiante(int c, const std::string& n, int e, const std::string& ca)
        : codigo(c), nombre(n), edad(e), carrera(ca) {}

    int getCodigo() const {
        return codigo;
    }

    void mostrar() const {
        std::cout << "Codigo: " << codigo << std::endl;
        std::cout << "Nombre: " << nombre << std::endl;
        std::cout << "Edad: " << edad << std::endl;
        std::cout << "Carrera: " << carrera << std::endl;
    }
};

void mostrarMenu();
void registrarEstudiante(std::vector<Estudiante>& estudiantes);
void mostrarEstudiantes(const std::vector<Estudiante>& estudiantes);
void buscarEstudiante(const std::vector<Estudiante>& estudiantes);

int main() {
    std::vector<Estudiante> estudiantes;
    int opcion;

    do {
        mostrarMenu();
        std::cin >> opcion;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (opcion) {
            case 1:
                registrarEstudiante(estudiantes);
                break;

            case 2:
                mostrarEstudiantes(estudiantes);
                break;

            case 3:
                buscarEstudiante(estudiantes);
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
    std::cout << "\n===== REGISTRO DE ESTUDIANTES =====" << std::endl;
    std::cout << "1. Registrar estudiante" << std::endl;
    std::cout << "2. Mostrar todos los estudiantes" << std::endl;
    std::cout << "3. Buscar estudiante por codigo" << std::endl;
    std::cout << "4. Salir" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void registrarEstudiante(std::vector<Estudiante>& estudiantes) {
    int codigo, edad;
    std::string nombre, carrera;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Codigo: ";
    std::cin >> codigo;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Nombre: ";
    std::getline(std::cin, nombre);

    std::cout << "Edad: ";
    std::cin >> edad;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Carrera: ";
    std::getline(std::cin, carrera);

    estudiantes.push_back(Estudiante(codigo, nombre, edad, carrera));

    std::cout << "Estudiante registrado correctamente." << std::endl;
}

void mostrarEstudiantes(const std::vector<Estudiante>& estudiantes) {
    if (estudiantes.empty()) {
        std::cout << "No hay estudiantes registrados." << std::endl;
        return;
    }

    for (const Estudiante& e : estudiantes) {
        e.mostrar();
        std::cout << "------------------------" << std::endl;
    }
}

void buscarEstudiante(const std::vector<Estudiante>& estudiantes) {
    int codigo;

    std::cout << "Ingrese el codigo: ";
    std::cin >> codigo;

    for (const Estudiante& e : estudiantes) {
        if (e.getCodigo() == codigo) {
            std::cout << "Estudiante encontrado" << std::endl;
            e.mostrar();
            return;
        }
    }

    std::cout << "No se encontro el estudiante." << std::endl;
}
