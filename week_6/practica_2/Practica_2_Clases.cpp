#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Empleado {
private:
    std::string nombre;
    std::string cargo;
    int antiguedad;

protected:
    double tarifaHora() const {
        if (cargo == "Gerente")
            return 100.0;
        else if (cargo == "Supervisor")
            return 80.0;
        else
            return 50.0; //Operador
    }

public:
    Empleado(const std::string& n, const std::string& c, int a)
        : nombre(n), cargo(c), antiguedad(a) {}

    virtual ~Empleado() {}

    std::string getNombre() const { return nombre; }
    std::string getCargo() const { return cargo; }
    int getAntiguedad() const { return antiguedad; }

    int prioridadCargo() const {
        if (cargo == "Gerente") return 1;
        else if (cargo == "Supervisor") return 2;
        return 3; //Operador
    }

    virtual double calcularSalario() const = 0;
    virtual std::string getTipo() const = 0;
    virtual void mostrar(std::ostream& os) const = 0;
};

class EmpleadoHora : public Empleado {
private:
    int horasTrabajadas;

public:
    EmpleadoHora(const std::string& n, const std::string& c, int a, int h)
        : Empleado(n, c, a), horasTrabajadas(h) {}

    double calcularSalario() const override {
        return tarifaHora() * horasTrabajadas;
    }

    std::string getTipo() const override {
        return "Por Hora";
    }

    void mostrar(std::ostream& os) const override {
        os << "Nombre: " << getNombre() << "\n";
        os << "Cargo: " << getCargo() << "\n";
        os << "Tipo: " << getTipo() << "\n";
        os << "Antiguedad: " << getAntiguedad() << " anios\n";
        os << "Horas trabajadas: " << horasTrabajadas << "\n";
        os << "Salario: $" << calcularSalario() << "\n";
    }
};

class EmpleadoPlanta : public Empleado {
public:
    EmpleadoPlanta(const std::string& n, const std::string& c, int a)
        : Empleado(n, c, a) {}

    double calcularSalario() const override {
        return tarifaHora() * 40;
    }

    std::string getTipo() const override {
        return "De Planta";
    }

    void mostrar(std::ostream& os) const override {
        os << "Nombre: " << getNombre() << "\n";
        os << "Cargo: " << getCargo() << "\n";
        os << "Tipo: " << getTipo() << "\n";
        os << "Antiguedad: " << getAntiguedad() << " anios\n";
        os << "Horas semanales: 40\n";
        os << "Salario: $" << calcularSalario() << "\n";
    }
};

void mostrarMayorSalario(const std::vector<Empleado*>& empleados, std::ostream& os);
void mostrarMayorTiempo(const std::vector<Empleado*>& empleados, std::ostream& os);
void ordenarPorCargo(std::vector<Empleado*>& empleados);
void guardarArchivo(const std::vector<Empleado*>& empleados);
void leerArchivo();

int main() {
    std::vector<Empleado*> empleados;

    empleados.push_back(new EmpleadoHora("Juan Perez", "Gerente", 8, 20));
    empleados.push_back(new EmpleadoPlanta("Nicoll Torres", "Supervisor", 12));
    empleados.push_back(new EmpleadoHora("Mike Wazowski", "Operario", 3, 10));
    empleados.push_back(new EmpleadoPlanta("Ana Ruiz", "Gerente", 10));
    empleados.push_back(new EmpleadoHora("Luis Soto", "Supervisor", 7, 25));
    empleados.push_back(new EmpleadoPlanta("Elena Vera", "Operario", 10));
    empleados.push_back(new EmpleadoHora("Pedro Rojas", "Operario", 5, 5));
    empleados.push_back(new EmpleadoPlanta("Sofia Luna", "Supervisor", 4));
    empleados.push_back(new EmpleadoHora("Miguel Gonzales", "Gerente", 9, 15));
    empleados.push_back(new EmpleadoPlanta("Laura Campos", "Operario", 6));

    guardarArchivo(empleados);
    leerArchivo();

    for (Empleado* e : empleados) {
        delete e;
    }

    return 0;
}

void mostrarMayorSalario(const std::vector<Empleado*>& empleados, std::ostream& os) {
    double mayorSalario = empleados[0]->calcularSalario();
    for (Empleado* e : empleados) {
        if (e->calcularSalario() > mayorSalario) {
            mayorSalario = e->calcularSalario();
        }
    }

    os << "\n===== MAYOR SALARIO =====\n";
    for (Empleado* e : empleados) {
        if (e->calcularSalario() == mayorSalario) {
            e->mostrar(os);
            os << "--------------------------\n";
        }
    }
}

void mostrarMayorTiempo(const std::vector<Empleado*>& empleados, std::ostream& os) {
    int mayorAntiguedad = empleados[0]->getAntiguedad();
    for (Empleado* e : empleados) {
        if (e->getAntiguedad() > mayorAntiguedad) {
            mayorAntiguedad = e->getAntiguedad();
        }
    }

    os << "\n===== MAYOR TIEMPO EN LA EMPRESA =====\n";
    for (Empleado* e : empleados) {
        if (e->getAntiguedad() == mayorAntiguedad) {
            e->mostrar(os);
            os << "--------------------------\n";
        }
    }
}

void ordenarPorCargo(std::vector<Empleado*>& empleados) {
    for (size_t i = 0; i < empleados.size() - 1; i++) {
        for (size_t j = 0; j < empleados.size() - i - 1; j++) {
            if (empleados[j]->prioridadCargo() > empleados[j + 1]->prioridadCargo()) {
                Empleado* aux = empleados[j];
                empleados[j] = empleados[j + 1];
                empleados[j + 1] = aux;
            }
        }
    }
}

void guardarArchivo(const std::vector<Empleado*>& empleados) {

    std::vector<Empleado*> empleadosCopia = empleados;
    ordenarPorCargo(empleadosCopia);
    
    std::ofstream archivo("empleados.txt");

    if (!archivo.is_open()) {
        std::cout << "Error al crear el archivo.\n";
        return;
    }

    archivo << "===== LISTA DE EMPLEADOS =====\n\n";
    for (Empleado* e : empleadosCopia) {
        e->mostrar(archivo);
        archivo << "--------------------------\n";
    }

    mostrarMayorSalario(empleadosCopia, archivo);
    mostrarMayorTiempo(empleadosCopia, archivo);

    archivo << "\n===== ORDENADOS POR CARGO =====\n";
    for (Empleado* e : empleadosCopia) {
        archivo << "(" << e->getCargo() << ") " << e->getNombre() 
                << " - " << e->getTipo() << "\n";
    }

    archivo.close();
}

void leerArchivo() {
    std::ifstream archivo("empleados.txt");
    
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo leer el archivo.\n";
        return;
    }
    
    std::string linea;
    
    while (getline(archivo, linea)) {
        std::cout << linea << "\n";
    }
    
    std::cout <<"\nArchivo guardado: empleados.txt\n";
    archivo.close();
}
