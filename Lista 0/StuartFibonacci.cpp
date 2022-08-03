#include <iostream>

using namespace std;

int fibonnaci(int num){
    if (num == 1) {
        return 0;
    }
    else if (num == 2) {
        return 1;
    }
    else {
        return (fibonnaci(num - 1) + fibonnaci(num - 2));
    }
}

int main() {
    int numero, resultado;
    cin >> numero;

    resultado = fibonnaci(numero);
    cout << resultado << endl;

    return 0;
}