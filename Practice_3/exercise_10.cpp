#include <iostream>
#include <string>
#include <vector>
#include <limits>

class Contacto {
private:
    std::string nombre;
    std::string telefono;
    std::string correo;

public:
    Contacto(const std::string& n, const std::string& t, const std::string& c)
        : nombre(n), telefono(t), correo(c) {}

    std::string getNombre() const { return nombre; }

    void mostrar() const {
        std::cout << "Nombre: " << nombre << std::endl;
        std::cout << "Telefono: " << telefono << std::endl;
        std::cout << "Correo: " << correo << std::endl;
    }
};

void mostrarMenu();
void agregarContacto(std::vector<Contacto>& contactos);
void buscarContacto(const std::vector<Contacto>& contactos);
void eliminarContacto(std::vector<Contacto>& contactos);
void mostrarContactos(const std::vector<Contacto>& contactos);

int main() {
    std::vector<Contacto> contactos;
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
                agregarContacto(contactos);
                break;

            case 2:
                buscarContacto(contactos);
                break;

            case 3:
                eliminarContacto(contactos);
                break;

            case 4:
                mostrarContactos(contactos);
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
    std::cout << "\n===== CONTACTOS =====" << std::endl;
    std::cout << "1. Agregar contacto" << std::endl;
    std::cout << "2. Buscar por nombre" << std::endl;
    std::cout << "3. Eliminar contacto" << std::endl;
    std::cout << "4. Mostrar todos los contactos" << std::endl;
    std::cout << "5. Salir" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void agregarContacto(std::vector<Contacto>& contactos) {
    std::string nombre, telefono, correo;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Nombre: ";
    std::getline(std::cin, nombre);

    if (nombre.empty()) {
        std::cout << "Nombre invalido." << std::endl;
        return;
    }

    std::cout << "Telefono: ";
    std::getline(std::cin, telefono);

    std::cout << "Correo: ";
    std::getline(std::cin, correo);

    contactos.push_back(Contacto(nombre, telefono, correo));

    std::cout << "Contacto agregado correctamente." << std::endl;
}

void buscarContacto(const std::vector<Contacto>& contactos) {
    std::string nombre;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Ingrese nombre a buscar: ";
    std::getline(std::cin, nombre);

    for (const Contacto& c : contactos) {
        if (c.getNombre() == nombre) {
            std::cout << "\nContacto encontrado:" << std::endl;
            c.mostrar();
            return;
        }
    }

    std::cout << "No se encontro el contacto." << std::endl;
}

void eliminarContacto(std::vector<Contacto>& contactos) {
    std::string nombre;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Ingrese nombre a eliminar: ";
    std::getline(std::cin, nombre);

    for (size_t i = 0; i < contactos.size(); i++) {
        if (contactos[i].getNombre() == nombre) {
            contactos.erase(contactos.begin() + i);
            std::cout << "Contacto eliminado." << std::endl;
            return;
        }
    }

    std::cout << "Contacto no encontrado." << std::endl;
}

void mostrarContactos(const std::vector<Contacto>& contactos) {
    if (contactos.empty()) {
        std::cout << "No hay contactos registrados." << std::endl;
        return;
    }

    for (const Contacto& c : contactos) {
        c.mostrar();
        std::cout << "------------------------" << std::endl;
    }
}
