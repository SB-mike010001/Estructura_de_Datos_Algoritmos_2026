#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <algorithm>

using Estudiante = std::tuple<int, std::string, std::string, double>;

void agregarEstudiante(std::vector<Estudiante>& estudiantes, int id, const std::string& nombre, 
                       const std::string& escuela, double promedio);
void mostrarEstudiante(const Estudiante& estudiante);
void mostrarTodos(const std::vector<Estudiante>& estudiantes);
void buscarPorId(const std::vector<Estudiante>& estudiantes, int id);
void buscarPorPromedio(const std::vector<Estudiante>& estudiantes, double promedioMinimo);
void ordenarPorPromedio(std::vector<Estudiante>& estudiantes);

int main() {
    std::vector<Estudiante> estudiantes;
    
    agregarEstudiante(estudiantes, 1, "Angel C", "Ingeniería", 8.5);
    agregarEstudiante(estudiantes, 2, "Mike P", "Medicina", 9.2);
    agregarEstudiante(estudiantes, 3, "Nicoll A", "Derecho", 7.8);
    agregarEstudiante(estudiantes, 4, "Juan P", "Ingeniería", 8.9);
    agregarEstudiante(estudiantes, 5, "Laura M", "Arquitectura", 9.5);
    
    std::cout << "=== TODOS LOS ESTUDIANTES ===" << std::endl;
    mostrarTodos(estudiantes);
    
    std::cout << "\n=== BUSCAR ESTUDIANTE ID 3 ===" << std::endl;
    buscarPorId(estudiantes, 3);
    
    std::cout << "\n=== ESTUDIANTES CON PROMEDIO > 8.5 ===" << std::endl;
    buscarPorPromedio(estudiantes, 8.5);
    
    std::cout << "\n=== ESTUDIANTES ORDENADOS POR PROMEDIO ===" << std::endl;
    ordenarPorPromedio(estudiantes);
    mostrarTodos(estudiantes);
    
    return 0;
}

void agregarEstudiante(std::vector<Estudiante>& estudiantes, int id, const std::string& nombre, 
                       const std::string& escuela, double promedio) {
    estudiantes.push_back(std::make_tuple(id, nombre, escuela, promedio));
}

void mostrarEstudiante(const Estudiante& estudiante) {
    std::cout << "ID: " << std::get<0>(estudiante)
              << " | Nombre: " << std::get<1>(estudiante)
              << " | Escuela: " << std::get<2>(estudiante)
              << " | Promedio: " << std::get<3>(estudiante) << std::endl;
}

void mostrarTodos(const std::vector<Estudiante>& estudiantes) {
    for (const auto& est : estudiantes) {
        mostrarEstudiante(est);
    }
}

void buscarPorId(const std::vector<Estudiante>& estudiantes, int id) {
    bool encontrado = false;
    for (const auto& est : estudiantes) {
        if (std::get<0>(est) == id) {
            mostrarEstudiante(est);
            encontrado = true;
            break;
        }
    }
    if (!encontrado) {
        std::cout << "No se encontró estudiante con ID: " << id << std::endl;
    }
}

void buscarPorPromedio(const std::vector<Estudiante>& estudiantes, double promedioMinimo) {
    bool encontrado = false;
    for (const auto& est : estudiantes) {
        if (std::get<3>(est) >= promedioMinimo) {
            mostrarEstudiante(est);
            encontrado = true;
        }
    }
    if (!encontrado) {
        std::cout << "No hay estudiantes con promedio >= " << promedioMinimo << std::endl;
    }
}

void ordenarPorPromedio(std::vector<Estudiante>& estudiantes) {
    std::sort(estudiantes.begin(), estudiantes.end(), 
        [](const Estudiante& a, const Estudiante& b) {
            return std::get<3>(a) > std::get<3>(b); 
        });
}
