#ifndef RBTREE_H
#define RBTREE_H

#include "Pedido.h"
#include <string>
using namespace std;

// Mismo diseno visto en clase (ArbolRedBlack): enum Color, NodoRB con padre
// y un nodo centinela TNULL (negro) que representa todas las hojas.
// Diferencia: en lugar de un int, cada nodo guarda un Pedido*; el orden del
// arbol es cronologico (fecha, hora, idPedido).
enum Color { ROJO, NEGRO };

struct NodoRB {
    Pedido* dato;
    Color   color;
    NodoRB *izq, *der, *padre;
    NodoRB(Pedido* d) : dato(d), color(ROJO), izq(nullptr), der(nullptr), padre(nullptr) {}
};

class RBTree {
private:
    NodoRB* raiz;
    NodoRB* TNULL;      // centinela (hojas negras)
    int     tamano;

    // --- Estructura base (vistos en clase) ---
    NodoRB* minimo(NodoRB* n);
    void    rotacionIzquierda(NodoRB* x);
    void    rotacionDerecha(NodoRB* x);
    void    transplantar(NodoRB* u, NodoRB* v);

    // --- Insercion ---
    NodoRB* insertarBST(Pedido* p);
    void    repararInsercion(NodoRB* k);

    // --- Eliminacion ---
    void    repararEliminacion(NodoRB* x);
    void    eliminarNodoHelper(NodoRB* z);
    NodoRB* buscarNodo(const Pedido* p) const;

    // --- Recorridos y validacion ---
    void destruirArbol(NodoRB* n);
    int  alturaHelper(NodoRB* n) const;
    int  validarAlturaNegra(NodoRB* n) const;    // -1 si hay violacion
    void rangoHelper(NodoRB* n, const string& ini, const string& fin,
                     int& cantidad, double& monto, int& visitados) const;

    // Contexto del recorrido in-order con limite de resultados
    struct Recorrido {
        int   limite;
        int   contados;
        void (*visitar)(Pedido*, void*);
        void* ctx;
    };
    void inordenHelper(NodoRB* n, Recorrido& r) const;
    void recolectarHelper(NodoRB* n, Pedido** out, int& k) const;

public:
    RBTree();
    ~RBTree();

    RBTree(const RBTree&) = delete;
    RBTree& operator=(const RBTree&) = delete;

    void insertar(Pedido* p);               // O(log n)
    bool eliminar(const Pedido* p);         // O(log n); NO libera el Pedido
    bool contiene(const Pedido* p) const;   // O(log n)

    // In-Order (cronologico). Visita como maximo 'limite' pedidos
    // (limite < 0 = todos). Devuelve cuantos visito.
    int  inOrder(int limite, void (*visitar)(Pedido*, void*), void* ctx) const;

    // Consulta por rango de fechas [ini, fin] (YYYY-MM-DD, inclusivo).
    // Descarta ramas fuera de rango. 'visitados' = nodos revisados.
    void rango(const string& ini, const string& fin,
               int& cantidad, double& monto, int& visitados) const;

    // Copia los punteros de todos los pedidos (en orden) en 'out'.
    int  recolectar(Pedido** out) const;

    int  getTamano() const { return tamano; }
    int  altura() const { return alturaHelper(raiz); }
    bool esValidoRB() const;                // raiz negra + altura negra + orden
    long long memoriaBytes() const { return (long long)(tamano + 1) * sizeof(NodoRB); }
};

#endif
