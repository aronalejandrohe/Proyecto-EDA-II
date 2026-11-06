#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Pedido.h"
#include <string>
using namespace std;

struct HashNode {
    string   idPedido;
    Pedido*  pedido;
    HashNode* next;

    HashNode(string id, Pedido* p) : idPedido(id), pedido(p), next(nullptr) {}
};

class HashTable {
private:
    HashNode** tabla;
    int        capacidad;
    int        tamanoActual;

    int hashFunction(const string& id) const;

public:
    HashTable(int cap);
    ~HashTable();

    void    insertar(Pedido* p);
    Pedido* buscar(const string& idPedido) const;
    bool    eliminar(const string& idPedido);
    int     getTamano() const;
    void    imprimir() const;
};

#endif
