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

// Datos de colisiones para la discusion del informe
struct HashStats {
    int    capacidad;       // cantidad de buckets
    int    elementos;       // pedidos guardados
    int    bucketsUsados;   // buckets con al menos un pedido
    int    bucketsVacios;
    int    colisiones;      // pedidos que cayeron en un bucket ya ocupado
    int    maxCadena;       // lista mas larga
    double promedioCadena;  // largo promedio de las listas no vacias
    double factorCarga;     // elementos / capacidad
    int    rehashes;        // cuantas veces creció la tabla
};


class HashTable {
private:
    HashNode** tabla;
    int        capacidad;
    int        tamanoActual;
    int        numRehashes;

    int hashFunction(const string& id) const;
    void agrandar();                             // duplica la capacidad


public:
    HashTable(int capInicial = 1009);
    ~HashTable();

   // No se permite copiar la tabla (maneja memoria con punteros)
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    bool    insertar(Pedido* p);                   // false si el ID ya existe
    Pedido* buscar(const string& idPedido) const;  // nullptr si no existe
    bool    eliminar(const string& idPedido);      // NO libera el Pedido
    int     getTamano() const { return tamanoActual; }
    int     getCapacidad() const { return capacidad; }

    HashStats estadisticas() const;
    long long memoriaBytes() const;
    void      imprimir(int maxBuckets = 20) const;
};

#endif
