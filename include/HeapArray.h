#ifndef HEAPARRAY_H
#define HEAPARRAY_H

#include "Pedido.h"
using namespace std;

// Max-Heap de pedidos ordenado por nivelPrioridad (mayor primero).
// Desempate: el pedido mas antiguo (fecha/hora) sale antes, y luego el ID,
// de modo que el orden de salida es totalmente determinista.
// Cada Pedido guarda su posicion en 'heapIdx' => borrado arbitrario en O(log n).

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
    void intercambiar(int i, int j);   // actualiza heapIdx de ambos
    void crecer();

public:
    explicit HeapArray(int cap = 1024);
    ~HeapArray();

    HeapArray(const HeapArray&) = delete;
    HeapArray& operator=(const HeapArray&) = delete;

    void     insertar(Pedido* p);          // O(log n)
    Pedido*  extractMax();                 // O(log n)
    bool     eliminar(Pedido* p);          // O(log n), usa p->heapIdx
    Pedido*  verMax()      const;          // O(1)
    int      getTamano()   const { return cantidadNodos; }
    bool     estaVacio()   const { return cantidadNodos == 0; }
    bool     esHeapValido() const;         // verifica la propiedad de max-heap
    long long memoriaBytes() const { return (long long)capacidad * sizeof(Pedido*); }
    void     imprimir(int maxItems = 20) const;
};

#endif
