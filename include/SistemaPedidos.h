#ifndef SISTEMAPEDIDOS_H
#define SISTEMAPEDIDOS_H

#include "Pedido.h"
#include "HashTable.h"
#include "HeapArray.h"
#include "RBTree.h"
#include <string>
using namespace std;

struct TiemposCarga {
    double lecturaMs;
    double hashMs;
    double arbolMs;
    double heapMs;
    double totalMs;
    int    leidos;
    int    duplicados;
    int    invalidos;
    TiemposCarga() : lecturaMs(0), hashMs(0), arbolMs(0), heapMs(0),
                     totalMs(0), leidos(0), duplicados(0), invalidos(0) {}
};

class SistemaPedidos {
private:
    HashTable hash;
    HeapArray heap;
    RBTree    arbol;
    int       total;
    double    montoTotal;

    bool insertarEnEstructuras(Pedido* p, TiemposCarga* tc);

public:
    SistemaPedidos();
    ~SistemaPedidos();
    SistemaPedidos(const SistemaPedidos&) = delete;
    SistemaPedidos& operator=(const SistemaPedidos&) = delete;

    bool cargarCsv(const string& ruta, TiemposCarga& tc);

    bool registrar(Pedido* p);

    Pedido* buscar(const string& id) const { return hash.buscar(id); }

    Pedido* despacharSiguiente();

    int consultaOrdenada(int limite, void (*visitar)(Pedido*, void*), void* ctx) const {
        return arbol.inOrder(limite, visitar, ctx);
    }

    void consultaRango(const string& ini, const string& fin,
                       int& cantidad, double& monto, int& visitados) const {
        arbol.rango(ini, fin, cantidad, monto, visitados);
    }

    bool actualizarEstado(const string& id, const string& nuevoEstado);

    bool cancelar(const string& id);

    int    getTotal()      const { return total; }
    int    getPendientes() const { return heap.getTamano(); }
    double getMontoTotal() const { return montoTotal; }

    const HashTable& getHash()  const { return hash; }
    const HeapArray& getHeap()  const { return heap; }
    const RBTree&    getArbol() const { return arbol; }
};

#endif
