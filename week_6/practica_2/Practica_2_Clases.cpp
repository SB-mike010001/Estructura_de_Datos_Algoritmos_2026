#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <limits>

class Empleado {
private:
    std::string nombre;
    std::string cargo;
    int antiguedad;

protected:
    double tarifaHora() const {
        if (cargo == "Gerente") return 100.0;
        else if (cargo == "Supervisor") return 80.0;
        else return 50.0; //Operador
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
        else return 3; //Operador
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
        os << std::left << std::setw(20) << "Nombre:" << getNombre() << "\n";
        os << std::setw(20) << "Cargo:" << getCargo() << "\n";
        os << std::setw(20) << "Tipo:" << getTipo() << "\n";
        os << std::setw(20) << "Antiguedad:" << getAntiguedad() << " anios\n";
        os << std::setw(20) << "Horas trabajadas:" << horasTrabajadas << "\n";
        os << std::setw(20) << "Salario:" << "$" << std::fixed << std::setprecision(2) << calcularSalario() << "\n";
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
        os << std::left << std::setw(20) << "Nombre:" << getNombre() << "\n";
        os << std::setw(20) << "Cargo:" << getCargo() << "\n";
        os << std::setw(20) << "Tipo:" << getTipo() << "\n";
        os << std::setw(20) << "Antiguedad:" << getAntiguedad() << " anios\n";
        os << std::setw(20) << "Horas semanales:" << "40\n";
        os << std::setw(20) << "Salario:" << "$" << std::fixed << std::setprecision(2) << calcularSalario() << "\n";
    }
};

void limpiarBuffer();
void mostrarMenu();
void opcionVerListaEmpleados(const std::vector<Empleado*>& empleados);
void opcionVerMayorSalario(const std::vector<Empleado*>& empleados);
void opcionVerMayorTiempo(const std::vector<Empleado*>& empleados);
void opcionVerOrdenPorCargo(const std::vector<Empleado*>& empleados);
void opcionGuardarArchivo(const std::vector<Empleado*>& empleados);
void mostrarListaEmpleados(const std::vector<Empleado*>& empleados, std::ostream& os);
void mostrarMayorSalario(const std::vector<Empleado*>& empleados, std::ostream& os);
void mostrarMayorTiempo(const std::vector<Empleado*>& empleados, std::ostream& os);
void ordenarPorCargo(std::vector<Empleado*>& empleados);

int main() {
    std::vector<Empleado*> empleados;

    empleados.push_back(new EmpleadoHora("Juan Perez", "Gerente", 8, 20));
    empleados.push_back(new EmpleadoPlanta("Nicoll Torres", "Supervisor", 12));
    empleados.push_back(new EmpleadoHora("Mike Wazowski", "Operario", 3, 10));
    empleados.push_back(new EmpleadoPlanta("Eliot Anderson", "Gerente", 10));
    empleados.push_back(new EmpleadoHora("Luis Soto", "Supervisor", 7, 25));
    empleados.push_back(new EmpleadoPlanta("Elena Vera", "Operario", 10));
    empleados.push_back(new EmpleadoHora("Pedro Rojas", "Operario", 5, 5));
    empleados.push_back(new EmpleadoPlanta("Sofia Luna", "Supervisor", 4));
    empleados.push_back(new EmpleadoHora("Miguel Gonzales", "Gerente", 9, 15));
    empleados.push_back(new EmpleadoPlanta("Laura Campos", "Operario", 6));

    int opcion;
    
    do {
        mostrarMenu();
        std::cout << "\nSeleccione una opcion: ";
        std::cin >> opcion;
        
        limpiarBuffer();
        std::cout << "\n";
        
        switch(opcion) {
            case 1:
                opcionVerListaEmpleados(empleados);
                break;
            case 2:
                opcionVerMayorSalario(empleados);
                break;
            case 3:
                opcionVerMayorTiempo(empleados);
                break;
            case 4:
                opcionVerOrdenPorCargo(empleados);
                break;
            case 5:
                opcionGuardarArchivo(empleados);
                break;
            case 6:
                std::cout << "Saliendo del programa...\n";
                break;
            default:
                std::cout << "Opcion no valida. Intente de nuevo.\n";
                break;
        }
        
        if (opcion != 6) {
            std::cout << "\nPresione Enter para continuar...";
            std::cin.get();
            std::cout << "\n";
        }
        
    } while(opcion != 6);
    
    for (Empleado* e : empleados) {
        delete e;
    }
    
    return 0;
}

void limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void mostrarMenu() {
    std::cout << "\n========================================\n";
    std::cout << "        SISTEMA DE GESTION DE EMPLEADOS\n";
    std::cout << "========================================\n";
    std::cout << "1. Ver lista de empleados\n";
    std::cout << "2. Ver empleado con mayor salario\n";
    std::cout << "3. Ver empleado con mayor tiempo en la empresa\n";
    std::cout << "4. Ver orden por cargo de empleado\n";
    std::cout << "5. Guardar en un fichero .txt\n";
    std::cout << "6. Salir\n";
    std::cout << "========================================\n";
}

void opcionVerListaEmpleados(const std::vector<Empleado*>& empleados) {
    mostrarListaEmpleados(empleados, std::cout);
}

void opcionVerMayorSalario(const std::vector<Empleado*>& empleados) {
    mostrarMayorSalario(empleados, std::cout);
}

void opcionVerMayorTiempo(const std::vector<Empleado*>& empleados) {
    mostrarMayorTiempo(empleados, std::cout);
}

void opcionVerOrdenPorCargo(const std::vector<Empleado*>& empleados) {
    std::vector<Empleado*> empleadosCopia = empleados;
    ordenarPorCargo(empleadosCopia);
    
    std::cout << "\n===== EMPLEADOS ORDENADOS POR CARGO =====\n";
    std::cout << std::left << std::setw(25) << "CARGO" << std::setw(25) << "NOMBRE" << "TIPO\n";
    std::cout << std::string(70, '-') << "\n";
    for (Empleado* e : empleadosCopia) {
        std::cout << std::left << std::setw(25) << e->getCargo() 
                  << std::setw(25) << e->getNombre() 
                  << e->getTipo() << "\n";
    }
}

void opcionGuardarArchivo(const std::vector<Empleado*>& empleados) {
    std::string nombreArchivo;
    std::cout << "Ingrese el nombre del archivo (sin extension): ";
    std::getline(std::cin, nombreArchivo);
    
    nombreArchivo += ".txt";
    
    std::vector<Empleado*> empleadosCopia = empleados;
    ordenarPorCargo(empleadosCopia);
    
    std::ofstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        std::cout << "Error al crear el archivo.\n";
        return;
    }
    
    archivo << "========================================\n";
    archivo << "     REPORTE DE EMPLEADOS\n";
    archivo << "========================================\n\n";
    
    mostrarListaEmpleados(empleadosCopia, archivo);
    mostrarMayorSalario(empleadosCopia, archivo);
    mostrarMayorTiempo(empleadosCopia, archivo);
    
    archivo << "\n===== ORDENADOS POR CARGO =====\n";
    archivo << std::left << std::setw(25) << "CARGO" << std::setw(25) << "NOMBRE" << "TIPO\n";
    archivo << std::string(70, '-') << "\n";
    for (Empleado* e : empleadosCopia) {
        archivo << std::left << std::setw(25) << e->getCargo() 
                << std::setw(25) << e->getNombre() 
                << e->getTipo() << "\n";
    }
    
    archivo.close();
    std::cout << "\nArchivo guardado exitosamente como: " << nombreArchivo << "\n";
}

void mostrarListaEmpleados(const std::vector<Empleado*>& empleados, std::ostream& os) {
    os << "\n================= LISTA DE EMPLEADOS =================\n\n";
    for (size_t i = 0; i < empleados.size(); i++) {
        os << "EMPLEADO #" << (i + 1) << ":\n";
        empleados[i]->mostrar(os);
        os << "--------------------------------------------------\n";
    }
}

void mostrarMayorSalario(const std::vector<Empleado*>& empleados, std::ostream& os) {
    if (empleados.empty()) return;
    
    double mayorSalario = empleados[0]->calcularSalario();
    for (Empleado* e : empleados) {
        if (e->calcularSalario() > mayorSalario) {
            mayorSalario = e->calcularSalario();
        }
    }
    
    os << "\n========== EMPLEADO(S) CON MAYOR SALARIO ==========\n";
    os << "Salario maximo: $" << std::fixed << std::setprecision(2) << mayorSalario << "\n\n";
    
    for (Empleado* e : empleados) {
        if (e->calcularSalario() == mayorSalario) {
            e->mostrar(os);
            os << "------------------------------------------\n";
        }
    }
}

void mostrarMayorTiempo(const std::vector<Empleado*>& empleados, std::ostream& os) {
    if (empleados.empty()) return;
    
    int mayorAntiguedad = empleados[0]->getAntiguedad();
    for (Empleado* e : empleados) {
        if (e->getAntiguedad() > mayorAntiguedad) {
            mayorAntiguedad = e->getAntiguedad();
        }
    }
    
    os << "\n========== EMPLEADO(S) CON MAYOR TIEMPO ==========\n";
    os << "Antiguedad maxima: " << mayorAntiguedad << " anios\n\n";
    
    for (Empleado* e : empleados) {
        if (e->getAntiguedad() == mayorAntiguedad) {
            e->mostrar(os);
            os << "------------------------------------------\n";
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
