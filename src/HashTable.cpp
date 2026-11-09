#include "../include/HashTable.h"
#include <iostream>
using namespace std;

// Funcion hash: convierte el ID en un numero de bucket.
// Idea: recorrer los caracteres y "mezclarlos" con h = h * 31 + caracter.
// Multiplicar por 31 hace que el ORDEN de los caracteres importe
// (a diferencia de solo sumarlos), asi PD-000012 y PD-000021 caen en
// buckets distintos.
int HashTable::hashFunction(const string& id) const {
    unsigned int h = 0;
    for (int i = 0; i < (int)id.size(); i++)
        h = h * 31 + (unsigned char)id[i];
    return h % capacidad;     // el resto siempre queda entre 0 y capacidad-1
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
            delete actual;            // borra el nodo, NO el Pedido
            actual = siguiente;
        }
    }
    delete[] tabla;
}

// Cuando la tabla se llena, se crea una tabla del doble de tamano y se
// pasan los nodos a su nueva posicion (el bucket depende de la capacidad).
// Asi la tabla escala a millones de pedidos y las listas siguen cortas.
void HashTable::agrandar() {
    int capacidadVieja = capacidad;
    HashNode** tablaVieja = tabla;

    capacidad = capacidad * 2 + 1;          // el +1 la deja impar
    tabla = new HashNode*[capacidad];
    for (int i = 0; i < capacidad; i++)
        tabla[i] = nullptr;

    for (int i = 0; i < capacidadVieja; i++) {
        HashNode* actual = tablaVieja[i];
        while (actual != nullptr) {
            HashNode* siguiente = actual->next;
            int idx = hashFunction(actual->pedido->idPedido);   // con la capacidad nueva
            actual->next = tabla[idx];
            tabla[idx]   = actual;
            actual = siguiente;
        }
    }
    delete[] tablaVieja;
    numRehashes++;
}

bool HashTable::insertar(Pedido* p) {
    // 1) Si ya existe ese ID, no se inserta
    if (buscar(p->idPedido) != nullptr)
        return false;

    // 2) Si la tabla esta muy llena (mas de 75%), se agranda primero
    if ((tamanoActual + 1) * 4 > capacidad * 3)
        agrandar();

    // 3) Insertar al inicio de la lista del bucket
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
                tabla[index] = actual->next;    // era el primero de la lista
            else
                previo->next = actual->next;    // estaba en medio o al final
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
    s.elementos = tamanoActual;
    s.bucketsUsados = 0;
    s.maxCadena = 0;
    long long sumaCadenas = 0;

    for (int i = 0; i < capacidad; i++) {
        int largo = 0;
        for (HashNode* a = tabla[i]; a != nullptr; a = a->next)
            largo++;
        if (largo > 0) {
            s.bucketsUsados++;
            sumaCadenas += largo;
            if (largo > s.maxCadena) s.maxCadena = largo;
        }
    }
    s.bucketsVacios  = capacidad - s.bucketsUsados;
    s.colisiones     = tamanoActual - s.bucketsUsados;
    s.promedioCadena = s.bucketsUsados ? (double)sumaCadenas / s.bucketsUsados : 0.0;
    s.factorCarga    = (double)tamanoActual / capacidad;
    s.rehashes       = numRehashes;
    return s;
}

long long HashTable::memoriaBytes() const {
    return (long long)capacidad * sizeof(HashNode*)
         + (long long)tamanoActual * sizeof(HashNode);
}


void HashTable::imprimir(int maxBuckets) const {
    cout << "\n--- Tabla Hash (primeros " << maxBuckets << " buckets ocupados) ---" << endl;
    int mostrados = 0;
    for (int i = 0; i < capacidad && mostrados < maxBuckets; i++) {
        if (tabla[i] != nullptr) {
            cout << "[" << i << "] ";
            for (HashNode* a = tabla[i]; a != nullptr; a = a->next)
                cout << a->pedido->idPedido << " ";
            cout << endl;
            mostrados++;
        }
    }
}
