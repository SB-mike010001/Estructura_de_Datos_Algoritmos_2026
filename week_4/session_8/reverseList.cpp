#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

std::vector<int> invertirLista(std::vector<int> lista);
void imprimir(const std::vector<int>& lista);

int main() {

  std::srand(std::time(NULL));

  std::vector<int> datos;
    datos.reserve(20);

    for (int i = 0; i < 20; i++) {
        datos.push_back(rand() % 100 + 1);
    }

    std::cout << "Lista original: ";
    imprimir(datos);

    std:: vector<int> invertida = invertirLista(datos);

    std::cout << "Lista invertida: ";
    imprimir(invertida);

    return 0;
}

std::vector<int> invertirLista(std::vector<int> lista) {
  std::reverse(lista.begin(), lista.end());
    return lista;
}

void imprimir(const std::vector<int>& lista) {
    for (int x : lista) {
      std::cout << x << " ";
    }
    std::cout << '\n';
}
