#include <iostream>
using namespace std;

void knapSack(int *w, int *v, int *b, int sizeK, int sizeN, int numVisits, int *vtVisits){
    int r[2][sizeK+1]; //MATRIZ COM 2 LINHAS, POIS SÓ PRECISA COMPARAR LINHA ATUAL(1) COM LINHA ANTERIOR(0)
    int tAux, vAux, wAux, factor;
    bool vAuxBool;

    //ZERAR LINHA 0 E COLUNA 0
    for (int j = 0; j <= sizeK; j++) {
        r[0][j] = 0;
    }
    r[1][0] = 0;
    
    for (int i = 1; i <= sizeN; i++) {
        for (int j = 1; j <= sizeK; j++) {

            //ITERAÇÃO ATUAL NÃO USA O ELEMENTO
            r[1][j] = r[0][j];

            //ITERAÇÃO ATUAL USA O ELEMENTO PELO MENOS 1 VEZ
            tAux = 0; //QUANTIDADE (t) DE REPETIÇÕES EM CADA BRINQUEDO
            vAux = 0; //DIVERTIMENTO DE (t) REPETIÇÕES EM CADA BRINQUEDO
            vAuxBool = true;
            while (vAuxBool) {
                tAux++;
                factor = v[i-1] - ((tAux-1) * (tAux-1) * b[i-1]); //DIVERTIMENTO DA REPETIÇÃO NÚMERO (t) EM CADA BRINQUEDO
                wAux = tAux * w[i-1]; //CUSTO DE (t) REPETIÇÕES EM CADA BRINQUEDO
                if (factor > 0 and wAux <= j) {
                    vAux += factor;
                    if (vAux + r[0][j-wAux] > r[1][j]) {
                        r[1][j] = vAux + r[0][j-wAux];
                    } 
                }
                else {
                    vAuxBool = false;
                }
            }
        }
        //COPIAR A LINHA ATUAL PARA A LINHA PASSADA, PARA INICIAR PRÓXIMA ITERAÇÃO
        for (int j = 1; j <= sizeK; j++) {
            r[0][j] = r[1][j];
        }
    }
    //PRINTAR DIVERSÃO MÁXIMA EM CADA VISITA AO PARQUE
    for (int j = 0; j < numVisits; j++) {
        cout << j << ": " << r[1][vtVisits[j]] << endl;
    }
}

int main(){
    int size, visits, kMax;

    cin >> size; //NÚMERO DE BRINQUEDOS
    int s[size], b[size], c[size];
    for (int i = 0; i < size; i++) {
        cin >> s[i]; //ÍNDICE DE DIVERSÃO DE CADA BRINQUEDO
        cin >> b[i]; //FATOR DE ABORRECIMENTO DE CADA BRINQUEDO
        cin >> c[i]; //CUSTO DE CADA BRINQUEDO
    }

    cin >> visits; //NÚMERO DE VISITAS AO PARQUE
    kMax = 0;
    int k[visits];
    for (int i = 0; i < visits; i++) {
        cin >> k[i]; //TOTAL DE CRÉDITOS EM CADA VISITA
        if (k[i] > kMax) {
            kMax = k[i]; //VISITA COM MAIOR TOTAL DE CRÉDITOS
        }
    }

    knapSack(c, s, b, kMax, size, visits, k);

    return 0;
}