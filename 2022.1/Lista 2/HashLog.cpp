#include <iostream>
#include <vector>
using namespace std;

struct _conection {
    int timestamp;
    unsigned int client;
};
typedef struct _conection conection;

class HashTable {
private:
    vector<conection> *table;
    int size;
    int load_factor;
    int max_cap;
    int cap;
    bool rehashing;

public:
    //CONSTRUTOR:
    HashTable(int _size, int _load_factor);

    //DESTRUTOR
    ~HashTable();

    //GETTERS:

    //SETTERS:
    void set_size(int _size);
    
    //MÉTODOS:
    bool verify_factor();
    void rehash();
    void add_conection(int _timestamp, unsigned int _client); //Inserção binária
    void search_conection(int _timestamp); //Busca binária

};

//CONSTRUTOR HASHTABLE:
HashTable::HashTable(int _size, int _load_factor) {
    size = _size;
    load_factor = _load_factor;
    max_cap = size * load_factor;
    table = new vector<conection>[size];
    cap = 0;
    rehashing = false;
}

//DESTRUTOR HASHTABLE:
HashTable::~HashTable() {
    delete[] table;
}

//GETTERS HASHTABLE:

//SETTERS HASHTABLE:
void HashTable::set_size(int _size) {
    size = _size;
    max_cap = size * load_factor;
}

//MÉTODOS HASHTABLE:
bool HashTable::verify_factor() {
    if (cap > max_cap) {
        return true;
    }
    return false;
}

void HashTable::rehash() {
    rehashing = true;
    vector<conection> *aux;
    aux = table;

    int temp_size = size;
    set_size(size*2+1);

    table = new vector<conection>[size*2+1];

    cap = 0;
    for (int i = 0; i < temp_size; i++) {
        for (int j = 0; j < aux[i].size(); j++) {
            add_conection(aux[i][j].timestamp, aux[i][j].client);
        }
    }
    
    delete[] aux;
    rehashing = false;
}

void HashTable::add_conection(int _timestamp, unsigned int _client) {
    if (verify_factor()) {
        rehash();
    }

    conection temp;
    temp.client = _client;
    temp.timestamp = _timestamp;

    int idx = _timestamp % size;

    int left = 0;
    int right = table[idx].size()-1;
    int middle;

    if (table[idx].size() == 0) {
        table[idx].push_back(temp);
    }
    else if (_timestamp > table[idx][right].timestamp) {
        table[idx].push_back(temp);
    }
    else if (_timestamp < table[idx][left].timestamp) {
        table[idx].insert(table[idx].begin(), temp);
    }
    else {
        while (left < right) {
            middle = (left + right) / 2;
            if (_timestamp > table[idx][left].timestamp and _timestamp < table[idx][middle].timestamp) {
                right = middle - 1;
                if (_timestamp > table[idx][right].timestamp) {
                    table[idx].insert(table[idx].begin()+middle, temp);
                }
            }
            else if (_timestamp < table[idx][right].timestamp and _timestamp > table[idx][middle].timestamp) {
                left = middle + 1;
                if (_timestamp < table[idx][left].timestamp) {
                    table[idx].insert(table[idx].begin()+left, temp);
                }
            }
        }
    }
    cap++;

    if (!rehashing) {
        cout << idx << " " << table[idx].size() << endl;
    }
}

void HashTable::search_conection(int _timestamp) {
    int idx = _timestamp % size;

    int left = 0;
    int right = table[idx].size()-1;
    int middle;

    if (!table[idx].empty()) {
        while (left <= right) {
            middle = (left + right) / 2;
            if (_timestamp == table[idx][middle].timestamp) {
                cout << table[idx][middle].client << " " << middle << endl;
                return;
            }
            else if (_timestamp < table[idx][middle].timestamp) {
                right = middle - 1;
            }
            else {
                left = middle + 1;
            }
        }
    }
    cout << "-1 -1" << endl;
}

int main(){
    int size, factor, timestamp;
    unsigned int client;
    string command;

    cin >> size;
    cin >> factor;

    HashTable hash(size, factor);

    while (command != "END") {
        cin >> command;

        if (command == "NEW") {
            cin >> timestamp;
            cin >> client;

            hash.add_conection(timestamp, client);
        }
        else if (command == "QRY") {
            cin >> timestamp;

            hash.search_conection(timestamp);
        }
    }
}