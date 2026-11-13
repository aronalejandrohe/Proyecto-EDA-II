#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Pedido.h"
#include <string>
using namespace std;

struct HashNode {
    Pedido*  pedido;
    HashNode* next;

    HashNode(Pedido* p) : pedido(p), next(nullptr) {}
};

struct HashStats {
    int    capacidad;
    int    bucketsUsados;
    int    colisiones;
    int    maxCadena;
    int    rehashes;
};


class HashTable {
private:
    HashNode** tabla;
    int        capacidad;
    int        tamanoActual;
    int        numRehashes;

    int hashFunction(const string& id) const;
    void agrandar();


public:
    HashTable(int capInicial = 1009);
    ~HashTable();

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    bool    insertar(Pedido* p);
    Pedido* buscar(const string& idPedido) const;
    bool    eliminar(const string& idPedido);
    int     getTamano() const { return tamanoActual; }
    int     getCapacidad() const { return capacidad; }

    HashStats estadisticas() const;
    long long memoriaBytes() const;
};

#endif

