#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <sstream>

double evaluarPostfija(const std::string& expresion);
bool esOperador(char c);

int main() {
    std::string expresion = "3 4 + 5 *";
    std::string expresion2 = "6 2 / 3 +";
    
    std::cout << "Expresion: " << expresion << " = " << evaluarPostfija(expresion) << std::endl;
    std::cout << "Expresion: " << expresion2 << " = " << evaluarPostfija(expresion2) << std::endl;
    
    return 0;
}

double evaluarPostfija(const std::string& expresion) {
    std::stack<double> pila;
    std::stringstream ss(expresion);
    std::string token;
    
    while (ss >> token) {

        if (isdigit(token[0]) || (token.length() > 1 && token[0] == '-')) {
            pila.push(std::stod(token));

        } else if (esOperador(token[0]) && token.length() == 1) {
            
          if (pila.size() < 2) {
                std::cerr << "Error: expresión inválida" << std::endl;
                return 0;
            }
            
            double b = pila.top(); pila.pop();
            double a = pila.top(); pila.pop();
            double resultado;
            
            switch(token[0]) {
                case '+': resultado = a + b; break;
                case '-': resultado = a - b; break;
                case '*': resultado = a * b; break;
                case '/': 
                    if (b == 0) {
                        std::cerr << "Error: división por cero" << std::endl;
                        return 0;
                    }
                    resultado = a / b; 
                    break;
                default:
                    std::cerr << "Error: operador no soportado" << std::endl;
                    return 0;
            }
            pila.push(resultado);
        } else {
            std::cerr << "Error: token inválido: " << token << std::endl;
            return 0;
        }
    }
    
    if (pila.size() != 1) {
        std::cerr << "Error: expresión inválida" << std::endl;
        return 0;
    }
    
    return pila.top();
}

bool esOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}
