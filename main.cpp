#include <iostream>
#include <string>
#include "TablaHash.h"
#include "MaxHeap.h"
using namespace std;

int main() {
    TablaHash hash(10007, 1);
    MaxHeap heap;
    Pedido* arr[8];
    int prios[8] = {3, 5, 1, 5, 2, 4, 5, 1};
    for (int i = 0; i < 8; i++) {
        arr[i] = new Pedido();
        arr[i]->idPedido = "PD-00000" + to_string(i + 1);
        arr[i]->nivelPrioridad = prios[i];
        arr[i]->fecha = "2026-06-01";
        arr[i]->hora = "08:00:0" + to_string(i);
        hash.insertar(arr[i]);
        heap.insertar(arr[i]);
    }
    cout << "Consistente: " << heap.esConsistente() << endl;
    heap.eliminar(arr[3]->nodoHeap);                 // cancelar uno del medio
    cout << "Consistente tras eliminar: " << heap.esConsistente() << endl;
    while (!heap.estaVacio()) {
        Pedido* p = heap.extraerMax();
        cout << p->idPedido << " prio " << p->nivelPrioridad << endl;
    }
    cout << "Buscar PD-000002: " << (hash.buscar("PD-000002") != nullptr) << endl;
    cout << "Buscar PD-999999: " << (hash.buscar("PD-999999") != nullptr) << endl;
    for (int i = 0; i < 8; i++) { hash.eliminar(arr[i]->idPedido); delete arr[i]; }
    return 0;
}
