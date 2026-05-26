#include <iostream>
#include <vector>
#include <set>

void imprimir(const std::vector<int>& lista);

int main() {

    std::vector<int> numeros = {
        5, 2, 8, 5, 1, 2, 9, 8, 7, 1
    };

    for(int n: numeros){
        std::cout<<n<<" ";
    }
        std::cout<<'\n';

    std::set<int> conjunto(
        numeros.begin(),
        numeros.end()
    );

    std::vector<int> sinDuplicados(
        conjunto.begin(),
        conjunto.end()
    );

    imprimir(sinDuplicados);

    return 0;
}

void imprimir(const std::vector<int>& lista) {

    for (int x : lista) {
        std::cout << x << " ";
    }

    std::cout << '\n';
}
