#ifndef RBTREE_H
#define RBTREE_H

#include "Pedido.h"
#include <string>
using namespace std;

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
    NodoRB* TNULL;
    int     tamano;

    NodoRB* minimo(NodoRB* n);
    void    rotacionIzquierda(NodoRB* x);
    void    rotacionDerecha(NodoRB* x);
    void    transplantar(NodoRB* u, NodoRB* v);

    NodoRB* insertarBST(Pedido* p);
    void    repararInsercion(NodoRB* k);

    void    repararEliminacion(NodoRB* x);
    void    eliminarNodoHelper(NodoRB* z);
    NodoRB* buscarNodo(const Pedido* p) const;

    void destruirArbol(NodoRB* n);
    int  alturaHelper(NodoRB* n) const;
    int  validarAlturaNegra(NodoRB* n) const;
    void rangoHelper(NodoRB* n, const string& ini, const string& fin,
                     int& cantidad, double& monto, int& visitados) const;

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

    void insertar(Pedido* p);
    bool eliminar(const Pedido* p);

    int  inOrder(int limite, void (*visitar)(Pedido*, void*), void* ctx) const;

    void rango(const string& ini, const string& fin,
               int& cantidad, double& monto, int& visitados) const;

    int  recolectar(Pedido** out) const;

    int  getTamano() const { return tamano; }
    int  altura() const { return alturaHelper(raiz); }
    bool esValidoRB() const;
    long long memoriaBytes() const { return (long long)(tamano + 1) * sizeof(NodoRB); }
};

#endif
