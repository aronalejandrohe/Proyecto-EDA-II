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

    void siftUp(int i);
    void siftDown(int i);
    void swap(int i, int j);

public:
    HeapArray(int cap);
    ~HeapArray();

    void     insertar(Pedido* p);
    Pedido*  extractMax();
    Pedido*  verMax()      const;
    int      getTamano()   const;
    bool     estaVacio()   const;
    void     imprimir()    const;
};

#endif
