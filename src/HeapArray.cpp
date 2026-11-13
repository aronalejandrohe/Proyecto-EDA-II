#include "../include/HeapArray.h"
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
    maxPedido->heapIdx = -1;
    return maxPedido;
}

bool HeapArray::eliminar(Pedido* p) {
    int i = p->heapIdx;
    if (i < 0 || i >= cantidadNodos || arr[i] != p) return false;

    cantidadNodos--;
    if (i != cantidadNodos) {
        Pedido* movido = arr[cantidadNodos];
        arr[i] = movido;
        movido->heapIdx = i;
        siftUp(i);
        siftDown(movido->heapIdx);
    }
    p->heapIdx = -1;
    return true;
}
