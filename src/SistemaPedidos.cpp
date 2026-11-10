#include "../include/SistemaPedidos.h"
#include "../include/CsvLoader.h"
#include "../include/Utils.h"
#include <fstream>
using namespace std;

SistemaPedidos::SistemaPedidos() : hash(1031), heap(1024) {
    total = 0;
    montoTotal = 0.0;
}

SistemaPedidos::~SistemaPedidos() {
    // El sistema es el dueno de los Pedido: se liberan una sola vez, recorriendo
    // el arbol, ANTES de que se destruyan las estructuras.
    if (total > 0) {
        Pedido** todos = new Pedido*[total];
        int n = arbol.recolectar(todos);
        for (int i = 0; i < n; i++)
            delete todos[i];
        delete[] todos;
    }
}

bool SistemaPedidos::insertarEnEstructuras(Pedido* p, TiemposCarga* tc) {
    Cronometro c;

    // 1) Hash primero: rechaza IDs duplicados antes de tocar el resto
    if (!hash.insertar(p)) return false;
    if (tc) { tc->hashMs += c.ms(); c.iniciar(); }

    // 2) Arbol Red-Black (todos los pedidos)
    arbol.insertar(p);
    if (tc) { tc->arbolMs += c.ms(); c.iniciar(); }

    // 3) Heap: solo los pedidos que esperan despacho (estado Pendiente)
    if (p->estado == "Pendiente")
        heap.insertar(p);
    if (tc) { tc->heapMs += c.ms(); }

    total++;
    montoTotal += p->montoDolares;
    return true;
}

bool SistemaPedidos::cargarCsv(const string& ruta, TiemposCarga& tc) {
    ifstream archivo(ruta.c_str());
    if (!archivo.is_open()) return false;

    tc = TiemposCarga();
    Cronometro global;
    Cronometro parcial;
    string linea;
    bool primero = true;

    while (getline(archivo, linea)) {
        parcial.iniciar();
        Pedido* p = parsearPedido(linea);            // nullptr si vacia o invalida
        tc.lecturaMs += parcial.ms();

        if (p == nullptr) {
            if (!linea.empty() && linea != "\r") tc.invalidos++;
            continue;
        }
        if (insertarEnEstructuras(p, &tc)) {
            tc.leidos++;
            if (primero) { primerId = p->idPedido; primero = false; }
            ultimoId = p->idPedido;
        } else {
            tc.duplicados++;
            delete p;                                // no entro: se libera
        }
    }
    tc.totalMs = global.ms();
    return true;
}

bool SistemaPedidos::registrar(Pedido* p) {
    return insertarEnEstructuras(p, nullptr);
}

Pedido* SistemaPedidos::despacharSiguiente() {
    Pedido* p = heap.extractMax();
    if (p != nullptr)
        p->estado = "EnTransito";     // mismo objeto que ven el hash y el arbol
    return p;
}

bool SistemaPedidos::actualizarEstado(const string& id, const string& nuevoEstado) {
    if (!estadoValido(nuevoEstado)) return false;
    Pedido* p = hash.buscar(id);
    if (p == nullptr) return false;

    p->estado = nuevoEstado;
    if (nuevoEstado == "Pendiente") {
        if (p->heapIdx < 0) heap.insertar(p);     // vuelve a la cola de despacho
    } else {
        if (p->heapIdx >= 0) heap.eliminar(p);    // ya no espera despacho
    }
    return true;
}

bool SistemaPedidos::cancelar(const string& id) {
    Pedido* p = hash.buscar(id);
    if (p == nullptr) return false;

    // Orden importante: el arbol necesita la clave (fecha/hora/id) del pedido,
    // por eso el pedido se libera AL FINAL.
    arbol.eliminar(p);
    if (p->heapIdx >= 0) heap.eliminar(p);
    hash.eliminar(id);
    total--;
    montoTotal -= p->montoDolares;
    delete p;
    return true;
}

bool SistemaPedidos::verificarConsistencia() const {
    return hash.getTamano() == total
        && arbol.getTamano() == total
        && heap.getTamano() <= total
        && arbol.esValidoRB()
        && heap.esHeapValido();
}
