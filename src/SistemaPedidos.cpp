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

    if (!hash.insertar(p)) return false;
    if (tc) { tc->hashMs += c.ms(); c.iniciar(); }

    arbol.insertar(p);
    if (tc) { tc->arbolMs += c.ms(); c.iniciar(); }

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

    while (getline(archivo, linea)) {
        parcial.iniciar();
        if (linea.compare(0, 9, "idPedido,") == 0) continue;
        Pedido* p = parsearPedido(linea);
        tc.lecturaMs += parcial.ms();

        if (p == nullptr) {
            if (!linea.empty() && linea != "\r") tc.invalidos++;
            continue;
        }
        if (insertarEnEstructuras(p, &tc)) {
            tc.leidos++;
        } else {
            tc.duplicados++;
            delete p;
        }
    }
    tc.totalMs = global.ms();
    return true;
}

bool SistemaPedidos::registrar(Pedido* p) {
    if (p == nullptr || !pedidoValido(*p)) return false;
    return insertarEnEstructuras(p, nullptr);
}

Pedido* SistemaPedidos::despacharSiguiente() {
    Pedido* p = heap.extractMax();
    if (p != nullptr)
        p->estado = "EnTransito";
    return p;
}

bool SistemaPedidos::actualizarEstado(const string& id, const string& nuevoEstado) {
    if (!estadoValido(nuevoEstado)) return false;
    Pedido* p = hash.buscar(id);
    if (p == nullptr) return false;

    p->estado = nuevoEstado;
    if (nuevoEstado == "Pendiente") {
        if (p->heapIdx < 0) heap.insertar(p);
    } else {
        if (p->heapIdx >= 0) heap.eliminar(p);
    }
    return true;
}

bool SistemaPedidos::cancelar(const string& id) {
    Pedido* p = hash.buscar(id);
    if (p == nullptr) return false;
    arbol.eliminar(p);
    if (p->heapIdx >= 0) heap.eliminar(p);
    hash.eliminar(id);
    total--;
    montoTotal -= p->montoDolares;
    delete p;
    return true;
}

