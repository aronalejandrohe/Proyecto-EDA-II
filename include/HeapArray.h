#ifndef HEAPARRAY_H
#define HEAPARRAY_H

#include "Pedido.h"
using namespace std;

class HeapArray {
private:
    Pedido** arr;
    int      capacidad;
    int      cantidadNodos;

    int  padre(int i)    const { return (i - 1) / 2; }
    int  hijoIzq(int i)  const { return 2 * i + 1;   }
    int  hijoDer(int i)  const { return 2 * i + 2;   }

    static bool tieneMasPrioridad(const Pedido* a, const Pedido* b);
    void siftUp(int i);
    void siftDown(int i);
    void intercambiar(int i, int j);
    void crecer();

public:
    explicit HeapArray(int cap = 1024);
    ~HeapArray();

    HeapArray(const HeapArray&) = delete;
    HeapArray& operator=(const HeapArray&) = delete;

    void     insertar(Pedido* p);
    Pedido*  extractMax();
    bool     eliminar(Pedido* p);
    int      getTamano()   const { return cantidadNodos; }
    bool     estaVacio()   const { return cantidadNodos == 0; }
    long long memoriaBytes() const { return (long long)capacidad * sizeof(Pedido*); }
};

#endif
