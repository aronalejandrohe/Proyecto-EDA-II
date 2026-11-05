#ifndef PEDIDO_H
#define PEDIDO_H

#include <string>
using namespace std;

struct NodoHeap;   // se define en MaxHeap.h

struct Pedido {
    string idPedido;
    string dniCliente;
    string tipoEnvio;
    int nivelPrioridad;
    double montoDolares;
    string fecha;       // YYYY-MM-DD
    string hora;        // HH:MM:SS
    string estado;      // Pendiente, Enviado, EnTransito, Entregado, Cancelado
    NodoHeap* nodoHeap; // nullptr si no está en el Heap

    Pedido() {
        nivelPrioridad = 0;
        montoDolares = 0;
        nodoHeap = nullptr;
    }
};

#endif
