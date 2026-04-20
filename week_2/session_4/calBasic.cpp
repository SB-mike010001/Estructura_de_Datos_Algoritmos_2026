#include <iostream>
using namespace std;

int main() {

  double a, b;
  char op;

  cout << "==== CALCULADORA BASICA '+,-,*,/' ====\n";
  
  cout << "Ingrese el primer numero:\n";
  cin >> a;

  cout << "Ingrese el operador '+,-,*,/':\n";
  cin >> op;

  cout << "Ingrese el segundo numero:\n";
  cin >> b;

  switch (op) {

    case '+':
      cout << a << " + " << b << " = " << a + b << endl;
      break;

    case '-':
      cout << a << " - " << b << " = " << a - b << endl;
      break;

    case '*':
      cout << a << " * " << b << " = " << a * b << endl;
      break;

    case '/':
      if (b != 0) {
        cout << a << " / " << b << " = " << a / b << endl;
      } else {
        cout << "No se puede dividir entre 0, intentelo de nuevo.\n";
      }
      break;

    default:
      cout << "Inserte un operador valido [+,-,*,/]\n";
      break;
  }

  return 0;
}
