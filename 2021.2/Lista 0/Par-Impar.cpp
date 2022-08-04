#include <iostream>

using namespace std;

int main()
{
    int num;
    cin >> num;

    if ((num % 2) == 0) {
        cout << "Par" << endl;
    }
    else {
        cout << "Impar" << endl;
    }
    
    return 0;
}