#include <iostream>
#include <queue>
#include <stack>

using namespace std;

struct _roundReturn {
    int idxPlayer = -1;
    int numCards = -1;
};
typedef struct _roundReturn roundReturn;

/*Considera o número da carta como as 
dezenase o naipe da carta como as unidades*/
int cardValue(){
    int ten, unit;
    char unitIn;

    cin >> ten;
    cin >> unitIn;

    if (unitIn == 'C') {
        unit = 1;
    }
    else if (unitIn == 'E') {
        unit = 2;
    }
    else if (unitIn == 'O') {
        unit = 3;
    }
    else if (unitIn == 'P') {
        unit = 4;
    }

    return (10 * ten + unit);
}

/*Adiciona nova carta para cada jogador ainda
na disputa (Jogadores com mão não vazia)*/
void addQueue(queue<int> *playerVt, int sizeQ, int round) {
    int val;
    for (int i = 0; i < sizeQ; i++) {
        if (!playerVt[i].empty() or round == 0) {
            val = cardValue();
            playerVt[i].push(val);
        }
    }
}

roundReturn addWinner(stack<int> center, queue<int> *playerVt, int idx) {
    roundReturn rt;
    int val;
    int counterCards = center.size();
    for (int i = 0; i < counterCards; i++) {
        val = center.top();
        center.pop();
        playerVt[idx].push(val);
    }
    rt.idxPlayer = idx;
    rt.numCards = counterCards;

    return rt;
}

/*Retira a primeira carta de cada jogador ainda na disputa (mão não vazia), adiciona as cartas à pilha
central e retorna o jogador que venceu a rodada e o número de cartas recebidas pelo jogador vencedor*/
roundReturn addStack(stack<int> center, queue<int> *playerVt, int sizeQ) {
    roundReturn rt;
    int val, valWinner=-1, idxWinner=-1, idxInGame, playersInGame=0;

    for (int i = 0; i < sizeQ; i++) {
        if (!playerVt[i].empty()) {
            val = playerVt[i].front();
            playerVt[i].pop();
            center.push(val);

            if (val > valWinner) {
                valWinner = val;
                idxWinner = i;
            }
            else if (val == valWinner) {
                idxWinner = -1;
            }
            if (!playerVt[i].empty()) {
                playersInGame++;
                idxInGame = i;
            }
        }
    }
    //Não teve empate
    if (idxWinner >= 0) {
        rt = addWinner(center, playerVt, idxWinner);
        return rt;
    }
    //Teve empate
    else {
        if (playersInGame == 0) {
            return rt;
        }
        else if (playersInGame == 1) {
            rt = addWinner(center, playerVt, idxInGame);
            return rt;
        }
        else {
            return addStack(center, playerVt, sizeQ);
        }
    }
}

int verifyWinner(queue<int> *players, int sizeQ){
    int winner, sizeWinner=-1, aux;
    for (int i = 0; i < sizeQ; i++) {
        aux = players[i].size();
        if (aux > sizeWinner) {
            winner = i;
            sizeWinner = aux;
        }
        else if (aux == sizeWinner) {
            winner = -1;
        }
    }
    return winner;
}

int main(){
    int totalPlayers, roundCounter=0;
    string commandI;
    roundReturn rnd;

    cin >> totalPlayers;
    queue<int> queuePlayers[totalPlayers];
    stack<int> center;

    while (commandI != "END") {
        cin >> commandI;
        if (commandI == "DEA") {
            addQueue(queuePlayers, totalPlayers, roundCounter);
        }
        else if (commandI == "RND") {
            rnd = addStack(center, queuePlayers, totalPlayers);
            if (rnd.idxPlayer >= 0) {
                cout << roundCounter << " " << rnd.idxPlayer << " " << rnd.numCards << endl;
            }
            else {
                cout << "-1 -1 -1" << endl;
            }
            roundCounter++;
        }
        else if (commandI == "END") {
            rnd.idxPlayer = verifyWinner(queuePlayers, totalPlayers);
        }
    }
    cout << rnd.idxPlayer << endl;
    
    return 0;
}