#include <iostream>
using namespace std;

struct _node {
    int val = 0;
    int weight = 0;
    struct _node *next = NULL;
};
typedef struct _node node;

struct _edge {
    node *point = NULL;
};
typedef struct _edge edge;

struct _heapElement {
    int val = 0;
    int weight = 0;
};
typedef struct _heapElement heapElmt;

struct _returnExtract {
    int val = 0;
    int weight = 0;
    int sizeHeap = 0;
};
typedef struct _returnExtract returnExtract;

struct _pathDR {
    int *dist = NULL;
    int *prec = NULL;
};
typedef struct _pathDR pathDR;

node* listInsert(node *cursor, int v, int w) {
    node *n;
    n = new node;

    n -> val = v;
    n -> weight = w;
    n -> next = cursor;
    cursor = n;

    return cursor;
}

node* listRemove(node *cursor) {
    if (cursor != NULL) {
        node *p = cursor;

        cursor = p -> next;
        delete p;
    }

    return cursor;
}

void swapElements(heapElmt *minHeap, int l, int r){
    int aux;

    aux = minHeap[l].val;
    minHeap[l].val = minHeap[r].val;
    minHeap[r].val = aux;

    aux = minHeap[l].weight;
    minHeap[l].weight = minHeap[r].weight;
    minHeap[r].weight = aux;
}

void bubbleUp(heapElmt *minHeap, int pos){
    int i, aux;

    i = pos;
    
    while (i > 0 and minHeap[i].weight <= minHeap[(i-1)/2].weight) {
        swapElements(minHeap, i, (i-1)/2);
        i = (i-1)/2;
    }
}

int heapInsert(heapElmt *minHeap, int sizeHeap, int v, int w){
    minHeap[sizeHeap].val = v;
    minHeap[sizeHeap].weight = w;

    bubbleUp(minHeap, sizeHeap);
    sizeHeap++;

    return sizeHeap;
}

void bubbleDown(heapElmt *minHeap, int pos, int sizeHeap){
    int l, r, m;

    l = 2 * pos + 1;
    r = 2 * pos + 2;
    m = pos;

    if (l < sizeHeap and minHeap[l].weight <= minHeap[m].weight) {
        m = l;
    }
    if (r < sizeHeap and minHeap[r].weight <= minHeap[m].weight) {
        m = r;
    }
    if (m != pos) {
        swapElements(minHeap, m, pos);
        return bubbleDown(minHeap, m, sizeHeap);
    }
}

void heapExtract(heapElmt *minHeap, returnExtract *aux, int sizeHeap){
    aux[0].val = minHeap[0].val;
    aux[0].weight = minHeap[0].weight;
    aux[0].sizeHeap = sizeHeap - 1;


    sizeHeap--;
    swapElements(minHeap, 0, sizeHeap);
    bubbleDown(minHeap, 0, sizeHeap);
}

int heapUpdate(heapElmt *minHeap, int sizeHeap, int v, int w){
    int i = 0;

    for (int i = 0; i < sizeHeap; i++) {
        if (minHeap[i].val == v) {
            minHeap[i].weight = w;
            bubbleUp(minHeap, i);
            return sizeHeap;
        }
    }
    sizeHeap = heapInsert(minHeap, sizeHeap, v, w);
    return sizeHeap;
}

void dijkstra(edge *grafo, int sizeG, int start, pathDR vt ){
    for (int i = 0; i < sizeG; i++) {
        vt.dist[i] = 2147483647;
        vt.prec[i] = 0;
    }

    vt.dist[start] = 0;
    returnExtract ext[1];
    heapElmt minHeap[sizeG];
    node *cursor;
    int u, du, sizeH, v, w;
    sizeH = heapInsert(minHeap, 0, start, 0);

    for (int i = 0; i < sizeG; i++) {
        heapExtract(minHeap, ext, sizeH);
        u = ext[0].val;
        du = ext[0].weight;
        sizeH = ext[0].sizeHeap;

        cursor = grafo[u].point;
        while (cursor != NULL) {
            v = cursor -> val;
            w = cursor -> weight;
            if ((du + w) < vt.dist[v]) {
                vt.dist[v] = du + w;
                vt.prec[v] = vt.prec[u] + 1;
                sizeH = heapUpdate(minHeap, sizeH, v, vt.dist[v]);
            }
            cursor = listRemove(cursor);
        }
    }
}

void printCase(int caseN, pathDR vt){
    int numMsg, emiter, receiver, sizeMsg, time, hops;

    cout << "caso " << caseN << ":" << endl;

    cin >> numMsg;
    for (int i = 0; i < numMsg; i++) {
        cin >> emiter;
        cin >> receiver;
        cin >> sizeMsg;

        hops = vt.prec[emiter] + vt.prec[receiver];
        time = (vt.dist[emiter] + vt.dist[receiver]) * sizeMsg / 1048576;

        cout << hops << " " << time << endl;
    }cout << endl;

}

void getGrafo(edge *grafo, int sizeG){
    int adj, sizeBytes, cost, numAdj;

    for (int j = 0; j < sizeG; j++) {
            cin >> numAdj;

            for (int k = 0; k < numAdj; k++) {
                cin >> adj;
                cin >> sizeBytes;
                cost = 1048576/sizeBytes;

                grafo[j].point = listInsert(grafo[j].point, adj, cost);
            }
        }
}

int main(){
    int numCase, size, dr;
    edge *grafo;
    pathDR vtDR;
    
    cin >> numCase;
    for (int i = 0; i < numCase; i++) {
        cin >> size;
        cin >> dr;

        vtDR.dist = new int[size];
        vtDR.prec = new int[size];
        grafo = new edge[size];

        getGrafo(grafo, size);
        dijkstra(grafo, size, dr, vtDR);
        printCase(i, vtDR);

        delete vtDR.dist;
        delete vtDR.prec;
        delete grafo;
    }

    return 0;
}