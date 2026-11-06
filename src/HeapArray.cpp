#include "../include/HeapArray.h"
#include <iostream>
using namespace std;

HeapArray::HeapArray(int cap) {
    capacidad     = cap;
    cantidadNodos = 0;
    arr           = new Pedido*[capacidad];
}

HeapArray::~HeapArray() {
    delete[] arr;
}

void HeapArray::swap(int i, int j) {
    Pedido* temp = arr[i];
    arr[i]       = arr[j];
    arr[j]       = temp;
}

void HeapArray::siftUp(int i) {
    while (i > 0 && arr[i]->nivelPrioridad > arr[padre(i)]->nivelPrioridad) {
        swap(i, padre(i));
        i = padre(i);
    }
}

void HeapArray::siftDown(int i) {
    int mayor = i;
    int izq   = hijoIzq(i);
    int der   = hijoDer(i);

    if (izq < cantidadNodos &&
        arr[izq]->nivelPrioridad > arr[mayor]->nivelPrioridad)
        mayor = izq;

    if (der < cantidadNodos &&
        arr[der]->nivelPrioridad > arr[mayor]->nivelPrioridad)
        mayor = der;

    if (mayor != i) {
        swap(i, mayor);
        siftDown(mayor);
    }
}

void HeapArray::insertar(Pedido* p) {
    if (cantidadNodos == capacidad) {
        cout << "ADVERTENCIA: Heap lleno, no se puede insertar "
             << p->idPedido << endl;
        return;
    }
    arr[cantidadNodos] = p;
    siftUp(cantidadNodos);
    cantidadNodos++;
}

Pedido* HeapArray::extractMax() {
    if (estaVacio()) {
        cout << "ADVERTENCIA: Heap vacio." << endl;
        return nullptr;
    }

    Pedido* maxPedido = arr[0];

    // Poner el ultimo elemento en la raiz y hacer siftDown
    arr[0] = arr[cantidadNodos - 1];
    cantidadNodos--;
    if (cantidadNodos > 0)
        siftDown(0);

    return maxPedido;
}

Pedido* HeapArray::verMax() const {
    if (estaVacio()) return nullptr;
    return arr[0];
}

int HeapArray::getTamano() const {
    return cantidadNodos;
}

bool HeapArray::estaVacio() const {
    return cantidadNodos == 0;
}

void HeapArray::imprimir() const {
    cout << "\n--- Heap (por prioridad) ---" << endl;
    for (int i = 0; i < cantidadNodos; i++) {
        cout << "[" << i << "] "
             << arr[i]->idPedido
             << " | Prior: " << arr[i]->nivelPrioridad
             << " | Tipo: "  << arr[i]->tipoEnvio
             << endl;
    }
}
