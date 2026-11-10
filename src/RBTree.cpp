#include "../include/RBTree.h"
using namespace std;

RBTree::RBTree() {
    TNULL = new NodoRB(nullptr);
    TNULL->color = NEGRO;
    TNULL->izq = TNULL->der = TNULL->padre = TNULL;
    raiz   = TNULL;
    tamano = 0;
}

RBTree::~RBTree() {
    destruirArbol(raiz);
    delete TNULL;
}

// Post-orden: libera los nodos (NO los Pedido)
void RBTree::destruirArbol(NodoRB* n) {
    if (n != TNULL) {
        destruirArbol(n->izq);
        destruirArbol(n->der);
        delete n;
    }
}

NodoRB* RBTree::minimo(NodoRB* n) {
    while (n->izq != TNULL) n = n->izq;
    return n;
}

// ------------------------------ Rotaciones -----------------------------
void RBTree::rotacionIzquierda(NodoRB* x) {
    NodoRB* y = x->der;
    x->der = y->izq;
    if (y->izq != TNULL) y->izq->padre = x;
    y->padre = x->padre;
    if (x->padre == nullptr) raiz = y;
    else if (x == x->padre->izq) x->padre->izq = y;
    else x->padre->der = y;
    y->izq = x;
    x->padre = y;
}

void RBTree::rotacionDerecha(NodoRB* x) {
    NodoRB* y = x->izq;
    x->izq = y->der;
    if (y->der != TNULL) y->der->padre = x;
    y->padre = x->padre;
    if (x->padre == nullptr) raiz = y;
    else if (x == x->padre->der) x->padre->der = y;
    else x->padre->izq = y;
    y->der = x;
    x->padre = y;
}

void RBTree::transplantar(NodoRB* u, NodoRB* v) {
    if (u->padre == nullptr) raiz = v;
    else if (u == u->padre->izq) u->padre->izq = v;
    else u->padre->der = v;
    v->padre = u->padre;     // se asigna incluso si v es TNULL (lo usa la reparacion)
}

// ------------------------------- Insercion -----------------------------
// Fase 1: insercion BST normal; el nodo nuevo entra ROJO
NodoRB* RBTree::insertarBST(Pedido* p) {
    NodoRB* nodo = new NodoRB(p);
    nodo->izq = TNULL;
    nodo->der = TNULL;

    NodoRB* y = nullptr;
    NodoRB* x = raiz;
    while (x != TNULL) {
        y = x;
        if (compararCronologico(*p, *x->dato) < 0) x = x->izq;
        else x = x->der;
    }
    nodo->padre = y;
    if (y == nullptr) raiz = nodo;
    else if (compararCronologico(*p, *y->dato) < 0) y->izq = nodo;
    else y->der = nodo;
    return nodo;
}

// Fase 2: reparar las propiedades Red-Black segun el color del tio
void RBTree::repararInsercion(NodoRB* k) {
    NodoRB* u;
    // (k->padre != nullptr) cubre el primer nodo: la raiz no tiene padre
    while (k->padre != nullptr && k->padre->color == ROJO) {
        if (k->padre == k->padre->padre->der) {
            u = k->padre->padre->izq;                // tio
            if (u->color == ROJO) {                  // Caso 1: tio rojo
                u->color = NEGRO;
                k->padre->color = NEGRO;
                k->padre->padre->color = ROJO;
                k = k->padre->padre;
            } else {
                if (k == k->padre->izq) {            // Caso 2: triangulo
                    k = k->padre;
                    rotacionDerecha(k);
                }
                k->padre->color = NEGRO;             // Caso 3: linea
                k->padre->padre->color = ROJO;
                rotacionIzquierda(k->padre->padre);
            }
        } else {                                     // simetrico
            u = k->padre->padre->der;
            if (u->color == ROJO) {
                u->color = NEGRO;
                k->padre->color = NEGRO;
                k->padre->padre->color = ROJO;
                k = k->padre->padre;
            } else {
                if (k == k->padre->der) {
                    k = k->padre;
                    rotacionIzquierda(k);
                }
                k->padre->color = NEGRO;
                k->padre->padre->color = ROJO;
                rotacionDerecha(k->padre->padre);
            }
        }
        if (k == raiz) break;
    }
    raiz->color = NEGRO;
}

void RBTree::insertar(Pedido* p) {
    NodoRB* nuevo = insertarBST(p);
    tamano++;
    repararInsercion(nuevo);
}

// ------------------------------ Eliminacion ----------------------------
void RBTree::repararEliminacion(NodoRB* x) {
    NodoRB* s;
    while (x != raiz && x->color == NEGRO) {
        if (x == x->padre->izq) {
            s = x->padre->der;                       // hermano
            if (s->color == ROJO) {                  // Caso 1
                s->color = NEGRO;
                x->padre->color = ROJO;
                rotacionIzquierda(x->padre);
                s = x->padre->der;
            }
            if (s->izq->color == NEGRO && s->der->color == NEGRO) {   // Caso 2
                s->color = ROJO;
                x = x->padre;
            } else {
                if (s->der->color == NEGRO) {        // Caso 3
                    s->izq->color = NEGRO;
                    s->color = ROJO;
                    rotacionDerecha(s);
                    s = x->padre->der;
                }
                s->color = x->padre->color;          // Caso 4
                x->padre->color = NEGRO;
                s->der->color = NEGRO;
                rotacionIzquierda(x->padre);
                x = raiz;
            }
        } else {                                     // simetrico
            s = x->padre->izq;
            if (s->color == ROJO) {
                s->color = NEGRO;
                x->padre->color = ROJO;
                rotacionDerecha(x->padre);
                s = x->padre->izq;
            }
            if (s->izq->color == NEGRO && s->der->color == NEGRO) {
                s->color = ROJO;
                x = x->padre;
            } else {
                if (s->izq->color == NEGRO) {
                    s->der->color = NEGRO;
                    s->color = ROJO;
                    rotacionIzquierda(s);
                    s = x->padre->izq;
                }
                s->color = x->padre->color;
                x->padre->color = NEGRO;
                s->izq->color = NEGRO;
                rotacionDerecha(x->padre);
                x = raiz;
            }
        }
    }
    x->color = NEGRO;
}

// Busca el nodo exacto de un pedido (por su clave cronologica)
NodoRB* RBTree::buscarNodo(const Pedido* p) const {
    NodoRB* tmp = raiz;
    while (tmp != TNULL) {
        int c = compararCronologico(*p, *tmp->dato);
        if (c == 0) return tmp;
        if (c < 0) tmp = tmp->izq;
        else tmp = tmp->der;
    }
    return TNULL;
}

void RBTree::eliminarNodoHelper(NodoRB* z) {
    NodoRB* x;
    NodoRB* y = z;
    Color y_color_original = y->color;

    if (z->izq == TNULL) {
        x = z->der;
        transplantar(z, z->der);
    } else if (z->der == TNULL) {
        x = z->izq;
        transplantar(z, z->izq);
    } else {
        y = minimo(z->der);                  // sucesor in-orden
        y_color_original = y->color;
        x = y->der;
        if (y->padre == z) x->padre = y;
        else {
            transplantar(y, y->der);
            y->der = z->der;
            y->der->padre = y;
        }
        transplantar(z, y);
        y->izq = z->izq;
        y->izq->padre = y;
        y->color = z->color;
    }
    delete z;
    tamano--;
    if (y_color_original == NEGRO) repararEliminacion(x);
}

bool RBTree::eliminar(const Pedido* p) {
    NodoRB* z = buscarNodo(p);
    if (z == TNULL) return false;
    eliminarNodoHelper(z);
    return true;
}

bool RBTree::contiene(const Pedido* p) const {
    return buscarNodo(p) != TNULL;
}

// ------------------------------- Consultas -----------------------------
// In-orden con corte temprano: se detiene al llegar a 'limite'
void RBTree::inordenHelper(NodoRB* n, Recorrido& r) const {
    if (n == TNULL) return;
    if (r.limite >= 0 && r.contados >= r.limite) return;
    inordenHelper(n->izq, r);
    if (r.limite >= 0 && r.contados >= r.limite) return;
    r.visitar(n->dato, r.ctx);
    r.contados++;
    inordenHelper(n->der, r);
}

int RBTree::inOrder(int limite, void (*visitar)(Pedido*, void*), void* ctx) const {
    Recorrido r;
    r.limite = limite;
    r.contados = 0;
    r.visitar = visitar;
    r.ctx = ctx;
    inordenHelper(raiz, r);
    return r.contados;
}

// Poda: si la fecha del nodo es < ini, TODO su subarbol izquierdo tambien lo
// es (se descarta); si es > fin, se descarta TODO el subarbol derecho.
void RBTree::rangoHelper(NodoRB* n, const string& ini, const string& fin,
                         int& cantidad, double& monto, int& visitados) const {
    if (n == TNULL) return;
    visitados++;
    const string& f = n->dato->fecha;
    if (f < ini) {
        rangoHelper(n->der, ini, fin, cantidad, monto, visitados);
    } else if (f > fin) {
        rangoHelper(n->izq, ini, fin, cantidad, monto, visitados);
    } else {
        cantidad++;
        monto += n->dato->montoDolares;
        rangoHelper(n->izq, ini, fin, cantidad, monto, visitados);
        rangoHelper(n->der, ini, fin, cantidad, monto, visitados);
    }
}

void RBTree::rango(const string& ini, const string& fin,
                   int& cantidad, double& monto, int& visitados) const {
    cantidad = 0;
    monto = 0.0;
    visitados = 0;
    rangoHelper(raiz, ini, fin, cantidad, monto, visitados);
}

void RBTree::recolectarHelper(NodoRB* n, Pedido** out, int& k) const {
    if (n == TNULL) return;
    recolectarHelper(n->izq, out, k);
    out[k++] = n->dato;
    recolectarHelper(n->der, out, k);
}

int RBTree::recolectar(Pedido** out) const {
    int k = 0;
    recolectarHelper(raiz, out, k);
    return k;
}

// ------------------------------- Validacion ----------------------------
int RBTree::alturaHelper(NodoRB* n) const {
    if (n == TNULL) return 0;
    int a = alturaHelper(n->izq);
    int b = alturaHelper(n->der);
    return 1 + (a > b ? a : b);
}

// Devuelve la altura negra, o -1 si alguna rama viola las propiedades
// (nodo rojo con hijo rojo, alturas negras distintas o orden BST incorrecto).
int RBTree::validarAlturaNegra(NodoRB* n) const {
    if (n == TNULL) return 1;
    if (n->izq != TNULL && compararCronologico(*n->izq->dato, *n->dato) >= 0) return -1;
    if (n->der != TNULL && compararCronologico(*n->der->dato, *n->dato) <= 0) return -1;
    int izqH = validarAlturaNegra(n->izq);
    int derH = validarAlturaNegra(n->der);
    if (izqH == -1 || derH == -1 || izqH != derH) return -1;
    if (n->color == ROJO && (n->izq->color == ROJO || n->der->color == ROJO)) return -1;
    return izqH + (n->color == NEGRO ? 1 : 0);
}

bool RBTree::esValidoRB() const {
    if (raiz == TNULL) return true;
    if (raiz->color != NEGRO) return false;
    return validarAlturaNegra(raiz) != -1;
}
