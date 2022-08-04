#include <iostream>

using namespace std;

struct _nodeTomb {
    int tombKey = -1;
    bool tombRemoved = false;
};
typedef struct _nodeTomb nodeTomb;

struct _nodeFloor {
    int numTombUsed = 0;
    nodeTomb *point;
};
typedef struct _nodeFloor nodeFloor;

//-Recebe a chave do indivíduo e o número de andares
//-Retorna o andar em que o indivíduo vai ser adicionado
int hashFunction(int key, int numFloors) {
    return (key % numFloors);
}

/*-Recebe o array ordenado, o número de espaços utilizado e o valor a ser adicionado
-Retorna o index do local a ser adicionado ou -1 se o valor já estiver no array*/
int binaryAdd(nodeTomb *vct, int tam, int value){
    int left = 0;
    int right = tam - 1;
    int middle;

    if (tam == 0) {
        return 0;
    }
    else if (value < vct[left].tombKey) {
        return 0;
    }
    else if (value > vct[right].tombKey) {
        return tam;
    }

    while (left < right) {
        middle = (left + right) / 2;
        if (value == vct[middle].tombKey or value == vct[left].tombKey or value == vct[right].tombKey) {
            return -1;
        }
        else if (value >= vct[left].tombKey and value < vct[middle].tombKey) {
            right = middle - 1;
            if (value > vct[right].tombKey) {
                return middle;
            }
        }
        else if (value <= vct[right].tombKey and value > vct[middle].tombKey) {        
            left = middle + 1;
            if (value < vct[left].tombKey) {
                return left;
            } 
        }
    }
    return middle;
}

/*-Recebe o array ordenado, o andar à ser adicionado, o tamanho
do array ordenado, o valor a ser adicionado, o número de andares,
um booleano indicando se o valor já foi removido (false como padraõ)
e um booleano indicando se essa adição é um rehashing (false como padrão)
-Retorna false se conseguir adicionar o indivíduo no cemitério
ou true não conseguir adicionar o indivíduo no cemitério
-Printa o andar e o jazigo do valor adicionado (se não for rehashing)*/
bool sortedAdd(nodeFloor *vct, int vctIndex, int tam, int value, 
int numFloors, bool doRemove=false, bool doRehash=false) {
    bool boolTravelVct = true;
    int intTravelVct = 0;

    if (doRemove) {
        return true;
    }

    while (intTravelVct < numFloors and boolTravelVct) {
        int idx = (vctIndex + intTravelVct) % numFloors;
        intTravelVct++;

        if (vct[idx].numTombUsed < tam) {
            int index = binaryAdd(vct[idx].point, vct[idx].numTombUsed, value);
            if (index >= 0) {
                for (int i = vct[idx].numTombUsed - 1; i >= index; i--) {
                    vct[idx].point[i+1] = vct[idx].point[i];
                }
                vct[idx].point[index].tombKey = value;
                vct[idx].point[index].tombRemoved = false;
                vct[idx].numTombUsed++;
            }
            boolTravelVct = false;
            if (doRehash == false) {
                cout << idx << "." << index << endl;
            }
        }
    }
    return boolTravelVct;
}

/*-Recebe o array ordenado, o número de espaços utilizados, 
o valor buscado e um booleano indicando se o valor deve ser removido
-Retorna o index se o valor buscado estiver no array, -2 se o valor buscado 
não estiver no array ou -1 se o valor buscado estava no array e foi removido */
int binarySearch(nodeTomb *vct, int tam, int value, bool doRemove){
    int left = 0;
    int right = tam - 1;
    int middle;

    while (left <= right) {
        middle = (left + right)/2;
        if (value == vct[middle].tombKey) {
            if (vct[middle].tombRemoved == false) {
                if (doRemove) {
                    vct[middle].tombRemoved = true;
                }
                return middle;
            }
            else {
                return -1;
            }
        }
        else if (value < vct[middle].tombKey) {
            right = middle - 1;
        }
        else {
            left = middle + 1;
        }
    }
    return -2; 
}

/*-Recebe o array ordenado, o andar inicial da busca, o número 
total de andares, o número total de jazigos, o valor buscado
e um booleano indicando se o indivíduo com esse valor deve ser removido
-Printa no console o andar e o jazigo do indivíduo buscado ou printa ?.?
se o indivíduo buscado não estiver em nenhum jazigo ou se já foi removido*/
void binarySearchR(nodeFloor *vct, int vctIndex, int numFloors, int numTombs, int value, bool doRemove) {
    int boolTravelVct = -2;
    int intTravelVct = 0;
    int idx = 0;
    while (intTravelVct < numFloors and boolTravelVct == -2) {
        idx = (vctIndex + intTravelVct) % numFloors;
        intTravelVct++;

        boolTravelVct = binarySearch(vct[idx].point, vct[idx].numTombUsed, value, doRemove);
        if (vct[idx].numTombUsed < numTombs and boolTravelVct == -2) {
            boolTravelVct = -1;
        }
    }
    if (boolTravelVct < 0) {
        cout << "?.?" << endl;
    }
    else {
        cout << idx << "." << boolTravelVct << endl;
    }
}

int main(){
    int numFloor, numFloorAux, numTomb, numKey, index;
    string commandInput;
    nodeFloor *cemetery;
    nodeFloor *cemeteryAux;
    bool doReHash = false;
    cin >> numFloor;
    cin >> numTomb;

    cemetery = new nodeFloor[numFloor];
    for (int i = 0; i < numFloor; i++) {
        cemetery[i].point = new nodeTomb[numTomb];
    }

    while (commandInput != "END"){
        cin >> commandInput;

        if (commandInput == "ADD"){
            cin >> numKey;
            index = hashFunction(numKey, numFloor);
            doReHash = sortedAdd(cemetery, index, numTomb, numKey, numFloor);

            if (doReHash) {
                doReHash = false;
                int numFloorAux, idx;
                cemeteryAux = new nodeFloor[numFloor];
                for (int i = 0; i < numFloor; i++) {
                    cemeteryAux[i].point = new nodeTomb[numTomb];
                    for (int j = 0; j < numTomb; j++) {
                        cemeteryAux[i].point[j].tombKey = cemetery[i].point[j].tombKey;
                        cemeteryAux[i].point[j].tombRemoved = cemetery[i].point[j].tombRemoved;
                    }
                    cemeteryAux[i].numTombUsed = cemetery[i].numTombUsed;
                    delete cemetery[i].point;
                }
                delete cemetery;
                
                numFloorAux = numFloor;
                numFloor = (numFloor * 2) + 1;

                cemetery = new nodeFloor[numFloor];
                for (int i = 0; i < numFloor; i++) {
                    cemetery[i].point = new nodeTomb[numTomb];
                }

                for (int i = 0; i < numFloorAux; i++) {
                    for (int j = 0; j < numTomb; j++) {
                        idx = hashFunction(cemeteryAux[i].point[j].tombKey, numFloor);
                        sortedAdd(cemetery, idx, numTomb, cemeteryAux[i].point[j].tombKey, numFloor, cemeteryAux[i].point[j].tombRemoved, true);
                    }
                    delete cemeteryAux[i].point;
                }
                delete cemeteryAux;

                idx = hashFunction(numKey, numFloor);
                sortedAdd(cemetery, idx, numTomb, numKey, numFloor);
            }
        }
        else if (commandInput == "REM"){
            cin >> numKey;
            index = hashFunction(numKey, numFloor);
            binarySearchR(cemetery, index, numFloor, numTomb, numKey, true);
        }
        else if (commandInput == "QRY"){
            cin >> numKey;
            index = hashFunction(numKey, numFloor);
            binarySearchR(cemetery, index, numFloor, numTomb, numKey, false);
        }
    }

    return 0;
}