#include "../include/HeapArray.h"
#include <iostream>
using namespace std;

HeapArray::HeapArray(int cap) {
    capacidad     = cap < 4 ? 4 : cap;
    cantidadNodos = 0;
    arr           = new Pedido*[capacidad];
}

HeapArray::~HeapArray() {
    delete[] arr;
}

// Orden total: prioridad desc, luego mas antiguo, luego ID
bool HeapArray::tieneMasPrioridad(const Pedido* a, const Pedido* b) {
    if (a->nivelPrioridad != b->nivelPrioridad)
        return a->nivelPrioridad > b->nivelPrioridad;
    return compararCronologico(*a, *b) < 0;
}

void HeapArray::intercambiar(int i, int j) {
    Pedido* temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
    arr[i]->heapIdx = i;
    arr[j]->heapIdx = j;
}

void HeapArray::siftUp(int i) {
    while (i > 0 && tieneMasPrioridad(arr[i], arr[padre(i)])) {
        intercambiar(i, padre(i));
        i = padre(i);
    }
}

void HeapArray::siftDown(int i) {
    while (true) {
        int mayor = i;
        int izq = hijoIzq(i);
        int der = hijoDer(i);
        if (izq < cantidadNodos && tieneMasPrioridad(arr[izq], arr[mayor])) mayor = izq;
        if (der < cantidadNodos && tieneMasPrioridad(arr[der], arr[mayor])) mayor = der;
        if (mayor == i) break;
        intercambiar(i, mayor);
        i = mayor;
    }
}

// Duplica el arreglo cuando se llena (soporte escalable a millones)
void HeapArray::crecer() {
    int nuevaCap = capacidad * 2;
    Pedido** nuevo = new Pedido*[nuevaCap];
    for (int i = 0; i < cantidadNodos; i++)
        nuevo[i] = arr[i];
    delete[] arr;
    arr = nuevo;
    capacidad = nuevaCap;
}

void HeapArray::insertar(Pedido* p) {
    if (cantidadNodos == capacidad) crecer();
    arr[cantidadNodos] = p;
    p->heapIdx = cantidadNodos;
    cantidadNodos++;
    siftUp(cantidadNodos - 1);
}

Pedido* HeapArray::extractMax() {
    if (estaVacio()) return nullptr;

    Pedido* maxPedido = arr[0];
    cantidadNodos--;
    if (cantidadNodos > 0) {
        arr[0] = arr[cantidadNodos];
        arr[0]->heapIdx = 0;
        siftDown(0);
    }
    maxPedido->heapIdx = -1;     // ya no esta en el heap
    return maxPedido;
}

// Borrado de un elemento cualquiera: se reemplaza por el ultimo y se
// reacomoda hacia arriba o hacia abajo segun corresponda.
bool HeapArray::eliminar(Pedido* p) {
    int i = p->heapIdx;
    if (i < 0 || i >= cantidadNodos || arr[i] != p) return false;

    cantidadNodos--;
    if (i != cantidadNodos) {
        Pedido* movido = arr[cantidadNodos];
        arr[i] = movido;
        movido->heapIdx = i;
        siftUp(i);                    // si sube, ya queda bien
        siftDown(movido->heapIdx);    // si no subio, quiza deba bajar
    }
    p->heapIdx = -1;
    return true;
}

Pedido* HeapArray::verMax() const {
    if (estaVacio()) return nullptr;
    return arr[0];
}

bool HeapArray::esHeapValido() const {
    for (int i = 0; i < cantidadNodos; i++) {
        if (arr[i]->heapIdx != i) return false;
        if (i > 0 && tieneMasPrioridad(arr[i], arr[padre(i)])) return false;
    }
    return true;
}

void HeapArray::imprimir(int maxItems) const {
    cout << "\n--- Heap (por prioridad) ---" << endl;
    for (int i = 0; i < cantidadNodos && i < maxItems; i++) {
        cout << "[" << i << "] "
             << arr[i]->idPedido
             << " | Prior: " << arr[i]->nivelPrioridad
             << " | Tipo: "  << arr[i]->tipoEnvio
             << endl;
    }
}
