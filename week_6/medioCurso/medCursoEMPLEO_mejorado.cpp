#include <iostream>
#include <iomanip>   
#include <string>
#include <fstream>
#include <limits>

// Variables globales
std::string nombre;
int horasTrabajadas = 0;
double salarioBase = 0;
double seguro = 0, afp = 0, comida = 0;
double bono1 = 0, bono2 = 0;
bool datosIngresados = false;  
const double TARIFA_POR_HORA = 120;

// Prototipo de Funciones
void calcularSalario();
void mostrarSalario();
void ingresarDescuentos();
void ingresarBeneficios();
void imprimirBoleta();
double calcularTotal();
void limpiarBuffer();
void guardarEnArchivo();
void generarBoleta(std::ostream& os);

int main() {
    char op;

    std::cout << "Nombre del empleado: ";
    std::getline(std::cin, nombre);

    do {  
        std::cout << "\n========= SISTEMA DE NOMINA =========\n";
        std::cout << "1) Ingresar horas trabajadas\n";
        std::cout << "2) Mostrar salario\n";
        std::cout << "3) Ingresar descuentos\n";
        std::cout << "4) Ingresar beneficios\n";
        std::cout << "5) Imprimir boleta\n";
        std::cout << "6) Salir\n";
        std::cout << "Opcion: ";
        std::cin >> op;
        limpiarBuffer();

        switch(op) {
            case '1': calcularSalario(); break;
            case '2': mostrarSalario(); break;
            case '3': ingresarDescuentos(); break;
            case '4': ingresarBeneficios(); break;
            case '5': imprimirBoleta(); break;
            case '6': std::cout << "Saliendo...\n"; break;
            default: std::cout << "Opcion invalida\n";
        }
    } while(op != '6');

    return 0;
}

void limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void calcularSalario() {
    std::cout << "Ingrese las horas trabajadas: ";
    std::cin >> horasTrabajadas;
    
    if(horasTrabajadas >= 0) {
        salarioBase = horasTrabajadas * TARIFA_POR_HORA;
        datosIngresados = true;
        std::cout << "Salario base calculado: $" << std::fixed << std::setprecision(2) << salarioBase << std::endl;
    } else {
        std::cout << "Error: Las horas trabajadas no pueden ser negativas\n";
    }
}

void mostrarSalario() {
    if(!datosIngresados) {
        std::cout << "\nPrimero debe ingresar las horas trabajadas (opcion 1)\n";
        return;
    }
    std::cout << "\nSalario base actual: $" << std::fixed << std::setprecision(2) << salarioBase << '\n';
}

void ingresarDescuentos() {
    if(!datosIngresados) {
        std::cout << "\nPrimero debe ingresar las horas trabajadas (opcion 1)\n";
        return;
    }
    
    std::cout << "\n--- Descuentos (puede poner 0 si no aplica) ---\n";
    std::cout << "Monto del seguro: $"; 
    std::cin >> seguro;
    if(seguro < 0) seguro = 0;
    
    std::cout << "Monto del AFP: $"; 
    std::cin >> afp;
    if(afp < 0) afp = 0;
    
    std::cout << "Monto de comida: $"; 
    std::cin >> comida;
    if(comida < 0) comida = 0;
    
    std::cout << "Descuentos registrados correctamente\n";
}

void ingresarBeneficios() {
    if(!datosIngresados) {
        std::cout << "\nPrimero debe ingresar las horas trabajadas (opcion 1)\n";
        return;
    }
    
    std::cout << "\n--- Beneficios (puede poner 0 si no aplica) ---\n";
    std::cout << "Monto del bono 1: $"; 
    std::cin >> bono1;
    if(bono1 < 0) bono1 = 0;
    
    std::cout << "Monto del bono 2: $"; 
    std::cin >> bono2;
    if(bono2 < 0) bono2 = 0;
    
    std::cout << "Beneficios registrados correctamente\n";
}

double calcularTotal() {
    double totalDescuentos = seguro + afp + comida;
    double totalBeneficios = bono1 + bono2;
    return salarioBase - totalDescuentos + totalBeneficios;
}

void generarBoleta(std::ostream& os) {
    double total = calcularTotal();
    
    // Configurar formato
    os << std::fixed << std::setprecision(2);
    
    // Encabezado
    os << "\n";
    os << "==================== BOLETA DE PAGO ====================\n";
    os << "Fecha: " << __DATE__ << " " << __TIME__ << "\n";
    os << "Empleado: " << nombre << "\n";
    os << "=========================================================\n\n";
    
    // Detalle de salario
    os << std::left << std::setw(30) << "Horas trabajadas:" << std::right << std::setw(15) << horasTrabajadas << " horas\n";
    os << std::left << std::setw(30) << "Tarifa por hora:" << std::right << std::setw(15) << TARIFA_POR_HORA << " $\n";
    os << std::left << std::setw(30) << "Salario base:" << std::right << std::setw(15) << salarioBase << " $\n";
    
    os << "\n";
    
    // Descuentos
    if(seguro > 0 || afp > 0 || comida > 0) {
        os << "-------------------- DESCUENTOS --------------------\n";
        if(seguro > 0) {
            os << std::left << std::setw(30) << "Seguro:" << std::right << std::setw(15) << seguro << " $\n";
        }
        if(afp > 0) {
            os << std::left << std::setw(30) << "AFP:" << std::right << std::setw(15) << afp << " $\n";
        }
        if(comida > 0) {
            os << std::left << std::setw(30) << "Comida:" << std::right << std::setw(15) << comida << " $\n";
        }
        double totalDescuentos = seguro + afp + comida;
        os << std::left << std::setw(30) << "Total descuentos:" << std::right << std::setw(15) << totalDescuentos << " $\n";
        os << "\n";
    }
    
    // Beneficios
    if(bono1 > 0 || bono2 > 0) {
        os << "-------------------- BENEFICIOS --------------------\n";
        if(bono1 > 0) {
            os << std::left << std::setw(30) << "Bono 1:" << std::right << std::setw(15) << bono1 << " $\n";
        }
        if(bono2 > 0) {
            os << std::left << std::setw(30) << "Bono 2:" << std::right << std::setw(15) << bono2 << " $\n";
        }
        double totalBeneficios = bono1 + bono2;
        os << std::left << std::setw(30) << "Total beneficios:" << std::right << std::setw(15) << totalBeneficios << " $\n";
        os << "\n";
    }
    
    // Total
    os << "=========================================================\n";
    os << std::left << std::setw(30) << "TOTAL NETO:" << std::right << std::setw(15) << total << " $\n";
    os << "=========================================================\n";
    
    // Advertencias
    if(total < 0) {
        os << "\nADVERTENCIA: Salario negativo! Los descuentos superan al salario.\n";
    } else if(total == 0) {
        os << "\nNOTA: El empleado no recibe pago este periodo.\n";
    }
    
    os << "\n";
}

void guardarEnArchivo() {
    std::ofstream archivo("boleta.txt");
    if(archivo.is_open()) {
        generarBoleta(archivo); 
        archivo.close();
        std::cout << "\nBoleta guardada en 'boleta.txt'\n";
    } else {
        std::cout << "\nError: No se pudo guardar el archivo\n";
    }
}

void imprimirBoleta() {
    if(!datosIngresados) {
        std::cout << "\nPrimero debe ingresar las horas trabajadas (opcion 1)\n";
        return;
    }
    
    generarBoleta(std::cout);
    guardarEnArchivo();
}
