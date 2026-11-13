#include "../include/HashTable.h"
using namespace std;

int HashTable::hashFunction(const string& id) const {
    unsigned int h = 0;
    for (int i = 0; i < (int)id.size(); i++)
        h = h * 31 + (unsigned char)id[i];
    return h % capacidad;
}

HashTable::HashTable(int capInicial) {
    capacidad    = capInicial < 11 ? 11 : capInicial;
    tamanoActual = 0;
    numRehashes  = 0;
    tabla        = new HashNode*[capacidad];
    for (int i = 0; i < capacidad; i++)
        tabla[i] = nullptr;
}

HashTable::~HashTable() {
    for (int i = 0; i < capacidad; i++) {
        HashNode* actual = tabla[i];
        while (actual != nullptr) {
            HashNode* siguiente = actual->next;
            delete actual;
            actual = siguiente;
        }
    }
    delete[] tabla;
}

void HashTable::agrandar() {
    int capacidadVieja = capacidad;
    HashNode** tablaVieja = tabla;

    capacidad = capacidad * 2 + 1;
    tabla = new HashNode*[capacidad];
    for (int i = 0; i < capacidad; i++)
        tabla[i] = nullptr;

    for (int i = 0; i < capacidadVieja; i++) {
        HashNode* actual = tablaVieja[i];
        while (actual != nullptr) {
            HashNode* siguiente = actual->next;
            int idx = hashFunction(actual->pedido->idPedido);
            actual->next = tabla[idx];
            tabla[idx]   = actual;
            actual = siguiente;
        }
    }
    delete[] tablaVieja;
    numRehashes++;
}

bool HashTable::insertar(Pedido* p) {
    if (buscar(p->idPedido) != nullptr)
        return false;

    if ((tamanoActual + 1) * 4 > capacidad * 3)
        agrandar();

    int index = hashFunction(p->idPedido);
    HashNode* nuevo = new HashNode(p);
    nuevo->next  = tabla[index];
    tabla[index] = nuevo;
    tamanoActual++;
    return true;
}

Pedido* HashTable::buscar(const string& idPedido) const {
    int index = hashFunction(idPedido);
    HashNode* actual = tabla[index];
    while (actual != nullptr) {
        if (actual->pedido->idPedido == idPedido)
            return actual->pedido;
        actual = actual->next;
    }
    return nullptr;
}

bool HashTable::eliminar(const string& idPedido) {
    int index = hashFunction(idPedido);
    HashNode* actual = tabla[index];
    HashNode* previo = nullptr;

    while (actual != nullptr) {
        if (actual->pedido->idPedido == idPedido) {
            if (previo == nullptr)
                tabla[index] = actual->next;
            else
                previo->next = actual->next;
            delete actual;
            tamanoActual--;
            return true;
        }
        previo = actual;
        actual = actual->next;
    }
    return false;
}

// Recorre la tabla y calcula los datos de colisiones (para el informe)
HashStats HashTable::estadisticas() const {
    HashStats s;
    s.capacidad = capacidad;
    s.bucketsUsados = 0;
    s.maxCadena = 0;

    for (int i = 0; i < capacidad; i++) {
        int largo = 0;
        for (HashNode* a = tabla[i]; a != nullptr; a = a->next)
            largo++;
        if (largo > 0) {
            s.bucketsUsados++;
            if (largo > s.maxCadena) s.maxCadena = largo;
        }
    }
    s.colisiones     = tamanoActual - s.bucketsUsados;
    s.rehashes       = numRehashes;
    return s;
}

long long HashTable::memoriaBytes() const {
    return (long long)capacidad * sizeof(HashNode*)
         + (long long)tamanoActual * sizeof(HashNode);
}
