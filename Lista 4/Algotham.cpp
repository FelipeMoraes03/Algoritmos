#include <iostream>
using namespace std;

struct _click {
    int idx = -1;
    int val = -1;
};
typedef struct _click click;

struct _agents {
    int father = -1;
    int height = 0;
    int *kClick = NULL;
};
typedef struct _agents agents;

struct _nodeI {
    int numConections = 0;
    int *point = NULL;
};
typedef struct _nodeI nodeI;

void createMatrix(nodeI *vt, int size){
    for (int i = 0; i < size; i++) {
        vt[i].point = new int[size];
        vt[i].point[i] = 0;
    }
} //T = θ(size)

int buildMatrix(nodeI *vt, int size, int* potentialClicks, int k){
    char p;
    int x;

    for (int i = 0; i < size; i++) {
        vt[i].numConections = 0;
        for (int j = 0; j < i; j++) {
            if (i != j) {
                cin >> p;
                x = p - 48; //ASCII

                vt[i].point[j] = x; 
                vt[j].point[i] = x; 

                if (x == 1) {
                    vt[i].numConections++;
                    vt[j].numConections++;
                }
            }
        }
    }

    int j = 0;
    for (int i = 0; i < size; i++) {
        if (vt[i].numConections >= k-1) {
            potentialClicks[j] = i;
            j++;
        }
    }
    return j;
} //T = θ(size²)

bool kCheck(nodeI* matrix, click* vt, int k){
    for (int i = 0; i < k-2; i++) {
        for (int j = i+1; j < k; j++) {
            if (matrix[vt[i].val].point[vt[j].val] == 0) {
                return false;
            }
        }
    }
    return true;
} //T = θ(k²)

void tryClick(nodeI* matrix, int* vt, click* aux, int n, int k){
    bool kClick;

    kClick = kCheck(matrix, aux, k);
    if (kClick) {
        return;
    }

    for (int x = 0; x <= k; x++) {
        int p = k - x;
        if (aux[p].idx < n-x) {
            aux[p].idx++;
            aux[p].val = vt[aux[p].idx];

            for (int y = 0; y < x; y++) {
                int q = k - y;
                if (aux[q].idx == n-y) {
                    aux[q].idx = aux[p].idx + x - y;
                    aux[q].val = vt[aux[q].idx];
                }
            }
            return tryClick(matrix, vt, aux, n, k);
        }
    }
}

void getClick(nodeI* matrix, int* vt, int* vtClick, int n, int k){
    click aux[k];
    for (int i = 0; i < k; i++) {
        aux[i].val = vt[i];
        aux[i].idx = i;
    }

    tryClick(matrix, vt, aux, n, k);

    for (int i = 0; i < k; i++) {
        vtClick[i] = aux[i].val;
    }

}

void checkAgent(nodeI* matrix, agents* vtAgents, int n, int k, int agent){
    int potentialNodes;
    int potentialClicks[n];

    if (vtAgents[agent].kClick == NULL) {
            vtAgents[agent].kClick = new int[k];
            potentialNodes = buildMatrix(matrix, n, potentialClicks, k);
            getClick(matrix, potentialClicks, vtAgents[agent].kClick, potentialNodes, k);
            vtAgents[agent].father = agent;
    }
}

int findFather(agents* vtAgents, int agent){
    int idx;

    if (vtAgents[agent].father == agent) {
        return agent;
    }
    idx = findFather(vtAgents, vtAgents[agent].father);
    vtAgents[agent].father = idx;
    return idx;
}

void unionClicks(agents* vtAgents, int x, int y){
    if (vtAgents[x].height < vtAgents[y].height) {
        vtAgents[x].father = y;
    }
    else if (vtAgents[y].height < vtAgents[x].height) {
        vtAgents[y].father = x;
    }
    else {
        vtAgents[y].father = x;
        vtAgents[x].height++;
    }
}

void printClick(int* kClick, int k){
    for (int i = 0; i < k; i++) {
        if (i == k-1) {
            cout << kClick[i] << endl;
        }
        else {
            cout << kClick[i] << " ";
        }
    }
}

void kClickComparator(agents* vtAgents, int agentX, int agentY, int k){
    int x, y;

    x = findFather(vtAgents, agentX);
    y = findFather(vtAgents, agentY);

    if (x == y) {
        cout << "SUCC ";
        printClick(vtAgents[x].kClick, k);
    }
    else {
        for (int i = 0; i < k; i++) {
            if (vtAgents[x].kClick[i] != vtAgents[y].kClick[i]) {
                cout << "FAIL" << endl;
                return;
            }
        }
        cout << "SUCC ";
        printClick(vtAgents[x].kClick, k);
        unionClicks(vtAgents, x, y);
    }
}

void deletePointers(agents* vtAgents, nodeI* vtMatrix, int numAgents, int sizeMatrix){
    
    for (int i = 0; i < numAgents; i++) {
        delete vtAgents[i].kClick;
    }
    for (int i = 0; i < sizeMatrix; i++) {
        delete vtMatrix[i].point;
    }
}

int main(){
    int numAgents, sizeMatrix, k, q, potentialNodes, agentX, agentY;

    cin >> numAgents;
    cin >> sizeMatrix;
    cin >> k;
    cin >> q;

    agents vtAgents[numAgents];
    nodeI vtMatrix[sizeMatrix];
    int potentialClicks[sizeMatrix];

    createMatrix(vtMatrix, sizeMatrix);

    for (int i = 0; i < q; i++) {
        cin >> agentX;
        cin >> agentY;

        checkAgent(vtMatrix, vtAgents, sizeMatrix, k, agentX);
        checkAgent(vtMatrix, vtAgents, sizeMatrix, k, agentY);
        kClickComparator(vtAgents, agentX, agentY, k);
    }

    deletePointers(vtAgents, vtMatrix, numAgents, sizeMatrix);

    return 0;
}