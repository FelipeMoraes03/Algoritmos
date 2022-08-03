#include <iostream>

using namespace std;

/*-Struct que representa cada elemento da árvore AVL*/
typedef struct _node node;
struct _node{
    int id;
    int bf = 0;

    int numBytesWEI;
    int numBytesRNK = 0;

    struct _node *pointL = NULL;
    struct _node *pointR = NULL;
};

/*-Struct responsável pelo retorno da função avlInsert*/
typedef struct _insertReturn insertR;
struct _insertReturn{
    node *point = NULL;
    int hModified = 0;
};

/*-Recebe um ponteiro para uma árvore AVL 
-Retorna o ponteiro da árvore AVL após a rotação à esquerda
(correção dos desbalanceamentos da altura da árvore)*/
node* rotationLeft(node *root) {
    root -> pointR -> numBytesRNK += (root -> numBytesRNK + root -> numBytesWEI);

    node *R = root -> pointR;
    node *RL = R -> pointL;
    R -> pointL = root;
    root -> pointR = RL;

    if (R -> bf <= 0) {
        root -> bf --;
    }
    else {
        root -> bf -= (R -> bf + 1);
    }

    if (root -> bf >= 0) {
        R -> bf --;
    }
    else {
        R -> bf += (root -> bf - 1);
    }

    return R;
}

/*-Recebe um ponteiro para uma árvore AVL 
-Retorna o ponteiro da árvore AVL após a rotação à direita
(correção dos desbalanceamentos da altura da árvore)*/
node* rotationRight(node *root) {
    root -> numBytesRNK -= (root -> pointL -> numBytesRNK + root -> pointL -> numBytesWEI);

    node *L = root -> pointL;
    node *LR = L -> pointR;
    L -> pointR = root;
    root -> pointL = LR;

    if (L -> bf >= 0) {
        root -> bf ++;
    }
    else {
        root -> bf += (1 - L -> bf);
    }

    if (root -> bf <= 0) {
        L -> bf ++;
    }
    else {
        L -> bf += 1 + root -> bf;
    }
    
    return L;
}

/*-Recebe um ponteiro para uma árvore AVL, o ID do nó a ser adicionado, o número de bytes
transmitidos pelo pacote com ID recebido, e uma struct responsável pelo retorno da função
-Retorna uma struct com o ponteiro da árvore AVL após a adição do pacote com ID recebido e
com um inteiro indicando se a altura da árvore foi alterada (alterada = 1)*/
insertR* avlInsert(node *root, int valueId, int valueWEI, insertR *doReturn){
    int hc;

    if (root == NULL) {
        node *N = new node;
        N -> id = valueId;
        N -> numBytesWEI = valueWEI;

        doReturn -> hModified = 1;
        doReturn -> point = N;
        return doReturn;
    }
    else if (valueId < root -> id) {
        root -> numBytesRNK += valueWEI;
        
        avlInsert(root -> pointL, valueId, valueWEI, doReturn);
        root -> pointL = doReturn -> point;
        hc = doReturn -> hModified;

        root -> bf -= hc;
    }
    else if (valueId > root -> id) {
        avlInsert(root -> pointR, valueId, valueWEI, doReturn);
        root -> pointR = doReturn -> point;
        hc = doReturn -> hModified;

        root -> bf += hc;
    }
    else {
        root -> numBytesWEI += valueWEI;

        doReturn -> hModified = 0;
        doReturn -> point = root;
        return doReturn;
    }
    
    if (hc == 0) {
        doReturn -> hModified = 0;
        doReturn -> point = root;
        return doReturn;
    }
    else if (root -> bf == 0) {
        doReturn -> hModified = 0;
        doReturn -> point = root;
        return doReturn;
    }
    else if (root -> bf == 1 or root -> bf == -1) {
        doReturn -> hModified = 1;
        doReturn -> point = root;
        return doReturn;
    }
    else {
        if (root -> bf == -2) {
            if (root -> pointL -> bf == 1) {
                root -> pointL = rotationLeft(root -> pointL);
            }
            doReturn -> hModified = 0;
            doReturn -> point = rotationRight(root);
            return doReturn;
        }
        else {
            if (root -> pointR -> bf == -1) {
                root -> pointR = rotationRight(root -> pointR);
            }
            doReturn -> hModified = 0;
            doReturn -> point = rotationLeft(root);
            return doReturn;
        }
    }
}

/*-Recebe um ponteiro para uma árvore AVL e um ID a ser buscado
-Printa no console o total de bytes transmitidos pelo pacote com o ID recebido
e a altura em que o nó do ID recebido se encontra na árvore AVL*/
void avlSearchWEI(node *root, int value, int height=0){
    if (root == NULL) {
        cout << "0 -1" << endl;
    }
    else if (value == root -> id) {
        cout << root -> numBytesWEI << " " << height << endl;
    }
    else if (value < root -> id) {
        height++;
        avlSearchWEI(root -> pointL, value, height);
    }
    else {
        height++;
        avlSearchWEI(root -> pointR, value, height);
    }
}

/*-Recebe um ponteiro para uma árvore AVL e um ID a ser buscado
-Printa no console o total de bytes transmitidos pelos pacotes com o ID inferior
ao ID recebido (Não necessariamente o ID recebido precisa estar árvore AVL)*/
void avlSearchRNK(node *root, int value, int previousRNK=0){
    if (root == NULL) {
        cout << previousRNK << endl;
    }
    else if (value == root -> id) {
        cout << root -> numBytesRNK + previousRNK << endl;
    }
    else if (value < root -> id) {
        avlSearchRNK(root -> pointL, value, previousRNK);
    }
    else {
        previousRNK += (root -> numBytesRNK + root -> numBytesWEI);

        avlSearchRNK(root -> pointR, value, previousRNK);
    }
}

/*-Recebe um ponteiro para um nó de uma árvore AVL
-Deleta essa árvore AVL*/
void avlClear(node *root){
    if (root == NULL) {
        return;
    }
    avlClear(root -> pointL);
    avlClear(root -> pointR);
    delete root;
}

int main(){
    node *systemGol = NULL;
    insertR *doReturn = new insertR;

    string commandInput;
    int id, numBytes, totalBytes = 0;

    while (commandInput != "END") {
        cin >> commandInput;

        if (commandInput == "ADD") {
            cin >> id;
            cin >> numBytes;

            avlInsert(systemGol, id, numBytes, doReturn);
            systemGol = doReturn -> point;
            totalBytes += numBytes;

            cout << totalBytes << endl;
        }
        else if (commandInput == "WEI") {
            cin >> id;

            avlSearchWEI(systemGol, id);
        }
        else if (commandInput == "RNK") {
            cin >> id;

            avlSearchRNK(systemGol, id);
        }
    }
    avlClear(systemGol);
    delete doReturn;

    return 0;
}