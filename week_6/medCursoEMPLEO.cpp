#include <iostream>
#include <iomanip>   // Para std::fixed y std::setprecision (formato de 2 decimales en dinero)
#include <string>

// Variables globales
std::string nombre;
double salarioBase = 0;
double seguro = 0, afp = 0, comida = 0;
double bono1 = 0, bono2 = 0;
bool datosIngresados = false;  

// Prototipo de Funciones
void ingresarDatos();
void mostrarSalario();
void ingresarDescuentos();
void ingresarBeneficios();
void imprimirBoleta();
double calcularTotal();
double validarSaldo(const std::string& mensaje);

int main() {
    char op;

    std::cout << "Bienvenido al Sistema de Nomina\n";
    std::cout << "Nombre del empleado: ";
    std::cin >> nombre;

    // BUCLE DO WHILE DEL MENU PARA QUE EL USUARIO SIGUE AÑADIENDO O DESCONTANDO SU SALDO
    do {  
        std::cout << "\n========= SISTEMA DE NOMINA =========\n";
        std::cout << "1) Ingresar salario base\n";
        std::cout << "2) Mostrar salario\n";
        std::cout << "3) Ingresar descuentos\n";
        std::cout << "4) Ingresar beneficios\n";
        std::cout << "5) Imprimir boleta\n";
        std::cout << "6) Salir\n";
        std::cout << "Opcion: ";
        std::cin >> op;

        switch(op) {
            case '1': ingresarDatos(); break;
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

// FUNCION PARA VALIDAR SALARIO POSITIVOS (> 0)
double validarSaldo(const std::string& mensaje) {
    double valor;
    do {
        std::cout << mensaje;
        std::cin >> valor;
        if(valor <= 0) {
            std::cout << "Error: El valor debe ser POSITIVO (> 0). Intente de nuevo.\n";
        }
    } while(valor <= 0);
    return valor;
}

// FUNCIONES 
void ingresarDatos() {
    // El salario base debe ser positivo (>0)
    salarioBase = validarSaldo("Salario base: ");
    datosIngresados = true;
}

void mostrarSalario() {
    if(salarioBase <= 0 && !datosIngresados) {
        std::cout << "\nPrimero debe ingresar el salario base (opcion 1)\n";
        return;
    }
    std::cout << "\nSalario base actual: $" << std::fixed << std::setprecision(2) << salarioBase << "\n";
}

void ingresarDescuentos() {
    if(salarioBase <= 0 && !datosIngresados) {
        std::cout << "\nPrimero debe ingresar el salario base (opcion 1)\n";
        return;
    }
    
    std::cout << "\n--- Descuentos (puede poner 0 si no aplica) ---\n";
    std::cout << "Seguro: "; std::cin >> seguro;
    if(seguro < 0) seguro = 0;
    
    std::cout << "AFP: "; std::cin >> afp;
    if(afp < 0) afp = 0;
    
    std::cout << "Comida: "; std::cin >> comida;
    if(comida < 0) comida = 0;
}

void ingresarBeneficios() {
    if(salarioBase <= 0 && !datosIngresados) {
        std::cout << "\nPrimero debe ingresar el salario base (opcion 1)\n";
        return;
    }
    
    std::cout << "\n--- Beneficios (puede poner 0 si no aplica) ---\n";
    std::cout << "Bono 1: "; std::cin >> bono1;
    if(bono1 < 0) bono1 = 0;
    
    std::cout << "Bono 2: "; std::cin >> bono2;
    if(bono2 < 0) bono2 = 0;
}

double calcularTotal() {
    double totalDescuentos = seguro + afp + comida;
    double totalBeneficios = bono1 + bono2;
    return salarioBase - totalDescuentos + totalBeneficios;
}

void imprimirBoleta() {
    if(salarioBase <= 0 && !datosIngresados) {
        std::cout << "\nPrimero debe ingresar el salario base (opcion 1)\n";
        return;
    }
    
    double total = calcularTotal();
    
    std::cout << "\n============= BOLETA =============\n";
    std::cout << "Nombre: " << nombre << "\n\n";
    
    // std::fixed + std::setprecision(2) para que el dinero tenga SIEMPRE 2 decimales
    std::cout << std::fixed << std::setprecision(2);
    
    std::cout << "Salario -------------------- $" << salarioBase << "\n\n";
    
    std::cout << "Descuentos\n";
    std::cout << "  Seguro:  ---------------- -$" << seguro << "\n";
    std::cout << "  AFP:     ---------------- -$" << afp << "\n";
    std::cout << "  Comida:  --------------- -$" << comida << "\n";
    
    std::cout << "\nBeneficios\n";
    std::cout << "  Bono 1:  ---------------- $" << bono1 << "\n";
    std::cout << "  Bono 2:  ---------------- $" << bono2 << "\n";
    
    std::cout << "\n----------------------------------\n";
    std::cout << "TOTAL: -------------------- $" << total << "\n";
    
    // Si el total es negativo, mostrar advertencia
    if(total < 0) {
        std::cout << "\nADVERTENCIA: El salario total es NEGATIVO\n";
        std::cout << "Los descuentos superan al salario + beneficios\n";
    }
    
    std::cout << "==================================\n";
}
