#include <iostream>
using namespace std;

void calBasic(const double num1, const double num2, const char op);

int main() {
  double a, b;
  char op;

  cout << "##### CALCULADORA BASICA '+,-,*,/' #####\n";
  cout << "Ejemplo: a + b -> ";

  if(!(cin >> a >> op >> b)) {
    cout << "Entrada invalida\n";
    return 1;
  }

  calBasic(a, b, op);
  cout << "########################################";
  return 0;
}

void calBasic(const double num1, const double num2, const char op) {
  double result;
  bool valid = true;

  switch(op) {
    case '+': result = num1 + num2; break;
    case '-': result = num1 - num2; break;
    case '*': result = num1 * num2; break;
    case '/':
      if(num2 != 0) result = num1 / num2;
      else valid = false;
      break;
    default:
      cout << "Operador invalido [+,-,*,/]\n";
      return;
  }

  if(valid)
    cout << num1 << " " << op << " " << num2 << " = " << result << '\n';
  else
    cout << "ERROR: NO SE PUEDE DIVIDIR ENTRE 0\n";
}
