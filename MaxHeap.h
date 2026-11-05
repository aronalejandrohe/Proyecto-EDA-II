#ifndef MAXHEAP_H
#define MAXHEAP_H

#include "Pedido.h"
using namespace std;

struct NodoHeap {
    Pedido* pedido;
    NodoHeap* izq;
    NodoHeap* der;
    NodoHeap* padre;

    NodoHeap(Pedido* p) {
        pedido = p;
        izq = nullptr;
        der = nullptr;
        padre = nullptr;
    }
};

class MaxHeap {
private:
    NodoHeap* raiz;
    int cantidadNodos;

    MaxHeap(const MaxHeap&) = delete;
    MaxHeap& operator=(const MaxHeap&) = delete;

    // true si 'a' debe estar por encima de 'b'
    bool mayorPrioridad(Pedido* a, Pedido* b) const {
        if (a->nivelPrioridad != b->nivelPrioridad)
            return a->nivelPrioridad > b->nivelPrioridad;
        if (a->fecha != b->fecha) return a->fecha < b->fecha;   // más antiguo primero
        if (a->hora != b->hora) return a->hora < b->hora;
        return a->idPedido < b->idPedido;                       // desempate reproducible
    }

    // Intercambia los pedidos de dos nodos Y actualiza nodoHeap de ambos
    void intercambiar(NodoHeap* a, NodoHeap* b) {
        Pedido* tmp = a->pedido;
        a->pedido = b->pedido;
        b->pedido = tmp;
        a->pedido->nodoHeap = a;
        b->pedido->nodoHeap = b;
    }

    // Nodo en la posición n (1 = raíz), navegando por los bits de n: O(log n)
    NodoHeap* obtenerNodo(int n) const {
        if (n < 1 || n > cantidadNodos) return nullptr;
        int mascara = 1;
        while ((mascara << 1) <= n) mascara <<= 1;   // bit más alto de n
        NodoHeap* nodo = raiz;
        mascara >>= 1;
        while (mascara > 0) {
            if (n & mascara) nodo = nodo->der;
            else nodo = nodo->izq;
            mascara >>= 1;
        }
        return nodo;
    }

    void siftUp(NodoHeap* n) {
        while (n->padre != nullptr && mayorPrioridad(n->pedido, n->padre->pedido)) {
            intercambiar(n, n->padre);
            n = n->padre;
        }
    }

    void siftDown(NodoHeap* n) {
        while (true) {
            NodoHeap* mayor = n;
            if (n->izq != nullptr && mayorPrioridad(n->izq->pedido, mayor->pedido))
                mayor = n->izq;
            if (n->der != nullptr && mayorPrioridad(n->der->pedido, mayor->pedido))
                mayor = n->der;
            if (mayor == n) break;
            intercambiar(n, mayor);
            n = mayor;
        }
    }

    void liberar(NodoHeap* n) {
        if (n == nullptr) return;
        liberar(n->izq);
        liberar(n->der);
        delete n;                       // solo el nodo, NUNCA el Pedido
    }

    bool verificar(NodoHeap* n) const {
        if (n == nullptr) return true;
        if (n->pedido->nodoHeap != n) return false;             // puntero de retorno
        if (n->izq != nullptr) {
            if (n->izq->padre != n) return false;
            if (mayorPrioridad(n->izq->pedido, n->pedido)) return false;
        }
        if (n->der != nullptr) {
            if (n->der->padre != n) return false;
            if (mayorPrioridad(n->der->pedido, n->pedido)) return false;
        }
        return verificar(n->izq) && verificar(n->der);
    }

public:
    MaxHeap() {
        raiz = nullptr;
        cantidadNodos = 0;
    }

    ~MaxHeap() { limpiarMemoria(); }

    void limpiarMemoria() {
        liberar(raiz);
        raiz = nullptr;
        cantidadNodos = 0;
    }

    void insertar(Pedido* p) {
        NodoHeap* nuevo = new NodoHeap(p);
        p->nodoHeap = nuevo;
        int pos = cantidadNodos + 1;
        if (raiz == nullptr) {
            raiz = nuevo;
        } else {
            NodoHeap* padre = obtenerNodo(pos / 2);
            nuevo->padre = padre;
            if (pos % 2 == 0) padre->izq = nuevo;
            else padre->der = nuevo;
        }
        cantidadNodos = pos;
        siftUp(nuevo);
    }

    // Elimina el nodo x (sirve para extraerMax y para cancelar). Devuelve su Pedido.
    Pedido* eliminar(NodoHeap* x) {
        if (x == nullptr) return nullptr;
        Pedido* p = x->pedido;
        NodoHeap* ultimo = obtenerNodo(cantidadNodos);
        if (x != ultimo) intercambiar(x, ultimo);   // ahora 'ultimo' guarda a p

        // desconectar el último nodo
        if (ultimo->padre == nullptr) raiz = nullptr;
        else if (ultimo->padre->izq == ultimo) ultimo->padre->izq = nullptr;
        else ultimo->padre->der = nullptr;

        p->nodoHeap = nullptr;
        delete ultimo;
        cantidadNodos--;

        if (x != ultimo) {                          // reparar el nodo que recibió al ex-último
            if (x->padre != nullptr && mayorPrioridad(x->pedido, x->padre->pedido))
                siftUp(x);
            else
                siftDown(x);
        }
        return p;
    }

    Pedido* extraerMax() {
        if (raiz == nullptr) return nullptr;
        return eliminar(raiz);
    }

    Pedido* verMax() const { return raiz == nullptr ? nullptr : raiz->pedido; }
    int getCantidad() const { return cantidadNodos; }
    bool estaVacio() const { return cantidadNodos == 0; }

    // Para pruebas: comprueba heap y puntero de retorno en todos los nodos
    bool esConsistente() const { return verificar(raiz); }
};

#endif
