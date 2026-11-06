#include "../include/CsvLoader.h"
#include "../include/HashTable.h"
#include "../include/HeapArray.h"
#include <iostream>
#include <ctime>
using namespace std;

// ─── Utilidad para medir tiempo ───────────────────────────
long long obtenerMs() {
    return (long long)clock() * 1000 / CLOCKS_PER_SEC;
}

// ─── Separador visual ─────────────────────────────────────
void separador(const string& titulo) {
    cout << "\n========================================" << endl;
    cout << "  " << titulo << endl;
    cout << "========================================" << endl;
}

int main() {

    // ─── PRUEBA 1: Carga CSV ──────────────────────────────
    separador("PRUEBA 1: CARGA DE CSV");

    long long t1 = obtenerMs();
    vector<Pedido*> pedidos = cargarCsv("data/pedidos_pequeno.csv");
    long long t2 = obtenerMs();

    cout << "Pedidos cargados : " << pedidos.size() << endl;
    cout << "Tiempo de carga  : " << (t2 - t1) << " ms" << endl;

    // ─── PRUEBA 2: Insercion en Hash ──────────────────────
    separador("PRUEBA 2: HASH TABLE");

    HashTable ht(500);

    t1 = obtenerMs();
    for (int i = 0; i < (int)pedidos.size(); i++)
        ht.insertar(pedidos[i]);
    t2 = obtenerMs();

    cout << "Pedidos insertados en hash : " << ht.getTamano() << endl;
    cout << "Tiempo de insercion        : " << (t2 - t1) << " ms" << endl;

    // Buscar primero del archivo
    cout << "\n-- Buscar PD-000001 (existe) --" << endl;
    t1 = obtenerMs();
    Pedido* encontrado = ht.buscar("PD-000001");
    t2 = obtenerMs();
    if (encontrado != nullptr) {
        encontrado->imprimir();
        cout << "Tiempo de busqueda: " << (t2 - t1) << " ms" << endl;
    }

    // Buscar ultimo del archivo
    cout << "\n-- Buscar PD-000030 (existe) --" << endl;
    Pedido* ultimo = ht.buscar("PD-000030");
    if (ultimo != nullptr)
        ultimo->imprimir();
    else
        cout << "No encontrado." << endl;

    // Buscar uno inexistente
    cout << "\n-- Buscar PD-999999 (no existe) --" << endl;
    Pedido* noExiste = ht.buscar("PD-999999");
    if (noExiste != nullptr)
        noExiste->imprimir();
    else
        cout << "No encontrado. (correcto)" << endl;

    // Eliminar uno
    cout << "\n-- Eliminar PD-000005 --" << endl;
    bool eliminado = ht.eliminar("PD-000005");
    cout << (eliminado ? "Eliminado OK" : "No encontrado") << endl;
    cout << "Pedidos en hash ahora: " << ht.getTamano() << endl;

    // Intentar eliminar el mismo otra vez
    cout << "\n-- Eliminar PD-000005 de nuevo (no debe existir) --" << endl;
    bool eliminado2 = ht.eliminar("PD-000005");
    cout << (eliminado2 ? "Eliminado OK" : "No encontrado. (correcto)") << endl;

    // ─── PRUEBA 3: Heap ───────────────────────────────────
    separador("PRUEBA 3: MAX-HEAP (DESPACHO POR PRIORIDAD)");

    HeapArray heap(500);

    t1 = obtenerMs();
    for (int i = 0; i < (int)pedidos.size(); i++)
        heap.insertar(pedidos[i]);
    t2 = obtenerMs();

    cout << "Pedidos insertados en heap : " << heap.getTamano() << endl;
    cout << "Tiempo de insercion        : " << (t2 - t1) << " ms" << endl;

    // Ver el de mayor prioridad sin extraer
    cout << "\n-- Pedido de mayor prioridad (sin extraer) --" << endl;
    Pedido* topMax = heap.verMax();
    if (topMax != nullptr)
        topMax->imprimir();

    // Despachar los 5 mas urgentes
    cout << "\n-- Despachando los 5 mas urgentes --" << endl;
    for (int i = 0; i < 5; i++) {
        Pedido* despachado = heap.extractMax();
        if (despachado != nullptr) {
            despachado->estado = "EnTransito";
            cout << "Despachado " << (i+1) << ": ";
            despachado->imprimir();
        }
    }
    cout << "Pedidos restantes en heap: " << heap.getTamano() << endl;

    // ─── PRUEBA 4: Verificacion cruzada Hash + Heap ───────
    separador("PRUEBA 4: VERIFICACION CRUZADA");

    cout << "Buscar en Hash un pedido despachado por Heap:" << endl;
    cout << "(El estado debe decir EnTransito si es el mismo objeto)" << endl;

    Pedido* verificar = ht.buscar("PD-000006");
    if (verificar != nullptr)
        verificar->imprimir();
    else
        cout << "No encontrado en hash." << endl;

    // ─── Resumen final ────────────────────────────────────
    separador("RESUMEN FINAL");
    cout << "Pedidos cargados del CSV : " << pedidos.size()   << endl;
    cout << "Pedidos en Hash          : " << ht.getTamano()   << endl;
    cout << "Pedidos en Heap          : " << heap.getTamano() << endl;

    // ─── Liberar memoria ──────────────────────────────────
    for (int i = 0; i < (int)pedidos.size(); i++)
        delete pedidos[i];

    return 0;
}
