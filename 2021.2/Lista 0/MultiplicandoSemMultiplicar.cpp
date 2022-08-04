#include <iostream>

using namespace std;

int main() 
{
    int num1, num2, resultado;

    cin >> num1;
    cin >> num2;
    resultado = 0;

    if (num2 >= 0) {
        for (int i = 0; i < num2; i++) {
            resultado += num1;
        }
    }
    else {
        for (int i = 0; i < - num2; i++) {
            resultado -= num1;
        }
    }
    
    cout << resultado << endl;

    return 0;
}