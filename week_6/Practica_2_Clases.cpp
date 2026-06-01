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
        else if (cargo == "Operador")
            return 80.0;
        else
            return 50.0; // Supervisor
    }

public:
    Empleado(const std::string& n, const std::string& c, int a)
        : nombre(n), cargo(c), antiguedad(a) {}

    virtual ~Empleado() {}

    std::string getNombre() const { return nombre; }
    std::string getCargo() const { return cargo; }
    int getAntiguedad() const { return antiguedad; }

    void setNombre(const std::string& n) { nombre = n; }
    void setCargo(const std::string& c) { cargo = c; }
    void setAntiguedad(int a) { antiguedad = a; }

    int prioridadCargo() const {
        if (cargo == "Gerente")
            return 1;

        if (cargo == "Operador")
            return 2;

        return 3; // Supervisor
    }

    virtual double calcularSalario() const = 0;
    virtual std::string getTipo() const = 0;
    virtual void mostrar() const = 0;
};

class EmpleadoHora : public Empleado {
private:
    int horasTrabajadas;

public:
    EmpleadoHora(const std::string& n,
                 const std::string& c,
                 int a,
                 int h)
        : Empleado(n, c, a), horasTrabajadas(h) {}

    ~EmpleadoHora() override {}

    int getHorasTrabajadas() const { return horasTrabajadas; }

    void setHorasTrabajadas(int h) { horasTrabajadas = h; }

    double calcularSalario() const override {
        return tarifaHora() * horasTrabajadas;
    }

    std::string getTipo() const override {
        return "Por Hora";
    }

    void mostrar() const override {
        std::cout << "Nombre: " << getNombre() << "\n";
        std::cout << "Cargo: " << getCargo() << "\n";
        std::cout << "Tipo: " << getTipo() << "\n";
        std::cout << "Antiguedad: " << getAntiguedad() << " anios\n";
        std::cout << "Horas trabajadas: " << horasTrabajadas << "\n";
        std::cout << "Salario: $" << calcularSalario() << "\n";
    }
};

class EmpleadoPlanta : public Empleado {
public:
    EmpleadoPlanta(const std::string& n,
                   const std::string& c,
                   int a)
        : Empleado(n, c, a) {}

    ~EmpleadoPlanta() override {}

    double calcularSalario() const override {
        return tarifaHora() * 40;
    }

    std::string getTipo() const override {
        return "De Planta";
    }

    void mostrar() const override {
        std::cout << "Nombre: " << getNombre() << "\n";
        std::cout << "Cargo: " << getCargo() << "\n";
        std::cout << "Tipo: " << getTipo() << "\n";
        std::cout << "Antiguedad: " << getAntiguedad() << " anios\n";
        std::cout << "Horas semanales: 40\n";
        std::cout << "Salario: $" << calcularSalario() << "\n";
    }
};

void mostrarMayorSalario(const std::vector<Empleado*>& empleados);
void mostrarMayorTiempo(const std::vector<Empleado*>& empleados);
void ordenarPorCargo(std::vector<Empleado*>& empleados);
void guardarReporteTXT(const std::vector<Empleado*>& empleados);

int main() {

    std::vector<Empleado*> empleados;

    empleados.push_back(new EmpleadoHora("Juan Perez", "Gerente", 8, 20));
    empleados.push_back(new EmpleadoPlanta("Nicoll Torres", "Supervisor", 12));
    empleados.push_back(new EmpleadoHora("Mike Wazowski", "Operador", 3, 10));
    empleados.push_back(new EmpleadoPlanta("Ana Ruiz", "Gerente", 15));
    empleados.push_back(new EmpleadoHora("Luis Soto", "Supervisor", 7, 25));
    empleados.push_back(new EmpleadoPlanta("Elena Vera", "Operador", 10));
    empleados.push_back(new EmpleadoHora("Pedro Rojas", "Operador", 5, 5));
    empleados.push_back(new EmpleadoPlanta("Sofia Luna", "Supervisor", 4));
    empleados.push_back(new EmpleadoHora("Miguel Gonzales", "Gerente", 9, 15));
    empleados.push_back(new EmpleadoPlanta("Laura Campos", "Operador", 6));

    std::cout << "===== LISTA DE EMPLEADOS =====\n\n";

    for (Empleado* e : empleados) {
        e->mostrar();
        std::cout << "--------------------------\n";
    }

    mostrarMayorSalario(empleados);
    mostrarMayorTiempo(empleados);

    ordenarPorCargo(empleados);

    std::cout << "\n===== ORDENADOS POR CARGO =====\n";

    for (Empleado* e : empleados) {
        std::cout
            << "("
            << e->getCargo()
            << ") "
            << e->getNombre()
            << " - "
            << e->getTipo()
            << "\n";
    }

    guardarReporteTXT(empleados);

    for (Empleado* e : empleados) {
        delete e;
    }

    return 0;
}

void mostrarMayorSalario(const std::vector<Empleado*>& empleados) {

    double mayorSalario = empleados[0]->calcularSalario();

    for (Empleado* e : empleados) {
        if (e->calcularSalario() > mayorSalario) {
            mayorSalario = e->calcularSalario();
        }
    }

    std::cout << "\n===== MAYOR SALARIO =====\n";

    for (Empleado* e : empleados) {
        if (e->calcularSalario() == mayorSalario) {
            e->mostrar();
            std::cout << "--------------------------\n";
        }
    }
}

void mostrarMayorTiempo(const std::vector<Empleado*>& empleados) {

    int mayorAntiguedad = empleados[0]->getAntiguedad();

    for (Empleado* e : empleados) {
        if (e->getAntiguedad() > mayorAntiguedad) {
            mayorAntiguedad = e->getAntiguedad();
        }
    }

    std::cout << "\n===== MAYOR TIEMPO EN LA EMPRESA =====\n";

    for (Empleado* e : empleados) {
        if (e->getAntiguedad() == mayorAntiguedad) {
            e->mostrar();
            std::cout << "--------------------------\n";
        }
    }
}

void ordenarPorCargo(std::vector<Empleado*>& empleados) {

    for (size_t i = 0; i < empleados.size() - 1; i++) {

        for (size_t j = 0; j < empleados.size() - i - 1; j++) {

            if (empleados[j]->prioridadCargo() >
                empleados[j + 1]->prioridadCargo()) {

                Empleado* aux = empleados[j];
                empleados[j] = empleados[j + 1];
                empleados[j + 1] = aux;
            }
        }
    }
}

void guardarReporteTXT(const std::vector<Empleado*>& empleados) {

    std::ofstream archivo("reporte_empleados.txt");

    if (!archivo.is_open()) {
        std::cout << "\nError al crear el archivo.\n";
        return;
    }

    archivo << "===== REPORTE DE EMPLEADOS =====\n\n";

    for (Empleado* e : empleados) {

        archivo << "Nombre: " << e->getNombre() << "\n";
        archivo << "Cargo: " << e->getCargo() << "\n";
        archivo << "Tipo: " << e->getTipo() << "\n";
        archivo << "Antiguedad: "
                << e->getAntiguedad()
                << " anios\n";
        archivo << "Salario: $"
                << e->calcularSalario()
                << "\n";
        archivo << "--------------------------\n";
    }

    archivo.close();

    std::cout << "\nReporte guardado exitosamente en "
              << "\"reporte_empleados.txt\"\n";
}
