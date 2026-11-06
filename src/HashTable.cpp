#include "../include/HashTable.h"
#include <iostream>
using namespace std;

// Funcion hash: suma de caracteres del ID mod capacidad
int HashTable::hashFunction(const string& id) const {
    int suma = 0;
    for (int i = 0; i < (int)id.size(); i++)
        suma += (int)id[i];
    return suma % capacidad;
}

HashTable::HashTable(int cap) {
    capacidad    = cap;
    tamanoActual = 0;
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

void HashTable::insertar(Pedido* p) {
    int index = hashFunction(p->idPedido);

    // Verificar si ya existe
    HashNode* actual = tabla[index];
    while (actual != nullptr) {
        if (actual->idPedido == p->idPedido) {
            cout << "ADVERTENCIA: " << p->idPedido
                 << " ya existe en la tabla hash." << endl;
            return;
        }
        actual = actual->next;
    }

    // Insertar al inicio de la lista
    HashNode* nuevo  = new HashNode(p->idPedido, p);
    nuevo->next      = tabla[index];
    tabla[index]     = nuevo;
    tamanoActual++;
}

Pedido* HashTable::buscar(const string& idPedido) const {
    int index        = hashFunction(idPedido);
    HashNode* actual = tabla[index];

    while (actual != nullptr) {
        if (actual->idPedido == idPedido)
            return actual->pedido;
        actual = actual->next;
    }
    return nullptr;  // No encontrado
}

bool HashTable::eliminar(const string& idPedido) {
    int index        = hashFunction(idPedido);
    HashNode* actual = tabla[index];
    HashNode* previo = nullptr;

    while (actual != nullptr) {
        if (actual->idPedido == idPedido) {
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
    return false;  // No encontrado
}

int HashTable::getTamano() const {
    return tamanoActual;
}

void HashTable::imprimir() const {
    cout << "\n--- Tabla Hash ---" << endl;
    for (int i = 0; i < capacidad; i++) {
        if (tabla[i] != nullptr) {
            cout << "[" << i << "] ";
            HashNode* actual = tabla[i];
            while (actual != nullptr) {
                cout << actual->idPedido << " ";
                actual = actual->next;
            }
            cout << endl;
        }
    }
}
