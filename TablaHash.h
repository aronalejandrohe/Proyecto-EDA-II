#ifndef TABLAHASH_H
#define TABLAHASH_H

#include <string>
#include "Pedido.h"
using namespace std;

struct NodoHash {
    string clave;          // idPedido
    Pedido* valor;
    NodoHash* siguiente;

    NodoHash(const string& c, Pedido* v) {
        clave = c;
        valor = v;
        siguiente = nullptr;
    }
};

class TablaHash {
private:
    NodoHash** tabla;
    int tamano;
    int cantidad;
    long colisiones;
    int tipoHash;          // 1 = suma de caracteres (profesor), 2 = polinomial

    int funcionHash(const string& clave) const {
        if (tipoHash == 1) {
            long suma = 0;
            for (size_t i = 0; i < clave.length(); i++)
                suma += (unsigned char)clave[i];
            return (int)(suma % tamano);
        }
        unsigned long h = 0;
        for (size_t i = 0; i < clave.length(); i++)
            h = (h * 31 + (unsigned char)clave[i]) % tamano;
        return (int)h;
    }

    // Evita copias accidentales (doble delete)
    TablaHash(const TablaHash&) = delete;
    TablaHash& operator=(const TablaHash&) = delete;

public:
    TablaHash(int tam = 10007, int tipo = 1) {
        tamano = tam;
        tipoHash = tipo;
        cantidad = 0;
        colisiones = 0;
        tabla = new NodoHash*[tamano];
        for (int i = 0; i < tamano; i++)
            tabla[i] = nullptr;
    }

    ~TablaHash() {
        for (int i = 0; i < tamano; i++) {
            NodoHash* actual = tabla[i];
            while (actual != nullptr) {
                NodoHash* sig = actual->siguiente;
                delete actual;          // solo el nodo, NUNCA el Pedido
                actual = sig;
            }
        }
        delete[] tabla;
    }

    void insertar(Pedido* p) {
        int pos = funcionHash(p->idPedido);
        if (tabla[pos] != nullptr) colisiones++;
        NodoHash* nuevo = new NodoHash(p->idPedido, p);
        nuevo->siguiente = tabla[pos];  // inserción al inicio
        tabla[pos] = nuevo;
        cantidad++;
    }

    Pedido* buscar(const string& id) const {
        int pos = funcionHash(id);
        NodoHash* actual = tabla[pos];
        while (actual != nullptr) {
            if (actual->clave == id) return actual->valor;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    bool eliminar(const string& id) {
        int pos = funcionHash(id);
        NodoHash* actual = tabla[pos];
        NodoHash* anterior = nullptr;
        while (actual != nullptr && actual->clave != id) {
            anterior = actual;
            actual = actual->siguiente;
        }
        if (actual == nullptr) return false;
        if (anterior == nullptr) tabla[pos] = actual->siguiente;   // era el primero
        else anterior->siguiente = actual->siguiente;
        delete actual;                  // solo el nodo
        cantidad--;
        return true;
    }

    // Para el informe (colisiones)
    int getCantidad() const { return cantidad; }
    int getTamano() const { return tamano; }
    long getColisiones() const { return colisiones; }

    int cadenaMasLarga() const {
        int maxLen = 0;
        for (int i = 0; i < tamano; i++) {
            int len = 0;
            for (NodoHash* a = tabla[i]; a != nullptr; a = a->siguiente) len++;
            if (len > maxLen) maxLen = len;
        }
        return maxLen;
    }
};

#endif
