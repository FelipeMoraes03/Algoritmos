#include <iostream>

using namespace std;

struct _node {
    string value;
    struct _node *point;
};

typedef struct _node node;

//Cria uma nova lista vazia
node* newList()
{
    node *head;
    head = new node;
    head -> point = nullptr;

    return head;
}

//Insere um novo elemento na posição indicada pelo ponteiro
node* listInsert(node *cursor, string val)
{
    node *n;
    n = new node;

    n -> value = val;
    n -> point = cursor -> point;
    cursor -> point = n;

    return cursor;
}

//Insere um novo elemento no final da fila
node* queueInsert(node *head, node *tail, string val)
{
    node *cursor = listInsert(tail, val);
    cursor = cursor -> point;

    return cursor;
}

//Retorna um ponteiro que aponta para o elemento de índice indicado
node* listPosition(node *head, int pos)
{
    int i = 0;
    node *cursor = head;

    while (i < pos and cursor -> point != nullptr) {
        cursor = cursor -> point;
        i++;
    }

    return cursor;
}

//Retorna o valor do elemento da lista a partir de seu índice
string listPositionValue(node *head, int pos)
{
    node *cursor;
    cursor = listPosition(head, pos);

    string i;
    if (cursor -> point != nullptr) {
        i = cursor -> point -> value;
    }
    else {
        i = "NULL";
    }

    return i;
}

//Remove o elemento da posição indicada pelo ponteiro
node* listRemove(node *cursor)
{
    if (cursor -> point != nullptr) {
        node *p = cursor -> point;

        cursor -> point = p -> point;
        delete p;
    }

        return cursor;
}

//Remove o 1° elemento da fila
node* queueRemove(node *head, node *tail)
{
    if (head != tail) {
        head = listRemove(head);
        if (head -> point == nullptr) {
            tail = head;
        } 
    }
    
    return tail;
}

//Remove todos os elementos da lista
node* emptyList(node *head, int listSize)
{
    for (int i = 0; i < listSize; i++) {
        head = listRemove(head);
    }

    return head;
}


int main()
{
    node *history = newList();
    node *savesHead = newList();
    node *savesTail = savesHead;
    string commandInput, url;
    int historySize = 0, savesSize = 0;

    while (commandInput != "END") {
        cin >> commandInput;

        if (commandInput == "OPN") {
            cin >> url;

            history = listInsert(history, url);
            historySize++;

            cout << historySize << endl;
        }
        else if (commandInput == "CUR") {
            cout << listPositionValue(history, 0) << endl;
        }
        else if (commandInput == "BCK") {
            history = listRemove(history);
            if (historySize > 0) {
            historySize--; 
            }

            cout << listPositionValue(history, 0) << endl;
        }
        else if (commandInput == "PAN") {
            history = emptyList(history, historySize);

            cout << historySize << endl;

            historySize = 0;
        }
        else if (commandInput == "SAV") {
            url = listPositionValue(history, 0);
            savesTail = queueInsert(savesHead, savesTail, url);
            savesSize++;

            cout << savesSize << endl;
        }
        else if (commandInput == "REA") {
            url = listPositionValue(savesHead, 0);
            savesTail = queueRemove(savesHead, savesTail);
            if (savesSize > 0) {
                savesSize--;
            }

            cout << url << endl;
        }
    }

    history = emptyList(history, historySize);
    free(history);
    savesHead = emptyList(savesHead, savesSize);
    free(savesHead);

    return 0;
}