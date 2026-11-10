#ifndef SISTEMAPEDIDOS_H
#define SISTEMAPEDIDOS_H

#include "Pedido.h"
#include "HashTable.h"
#include "HeapArray.h"
#include "RBTree.h"
#include <string>
using namespace std;

// Tiempos (ms) acumulados por estructura durante la carga masiva
struct TiemposCarga {
    double lecturaMs;   // leer archivo + parsear
    double hashMs;      // insertar en Tabla Hash
    double arbolMs;     // insertar en Red-Black
    double heapMs;      // insertar en Heap
    double totalMs;
    int    leidos;      // pedidos insertados correctamente
    int    duplicados;  // IDs repetidos descartados
    int    invalidos;   // lineas mal formadas descartadas
    TiemposCarga() : lecturaMs(0), hashMs(0), arbolMs(0), heapMs(0),
                     totalMs(0), leidos(0), duplicados(0), invalidos(0) {}
};

// Fachada que mantiene CONSISTENTES las tres estructuras:
//   - HashTable : busqueda por idPedido           O(1) promedio
//   - HeapArray : cola de despacho por prioridad  O(log n)
//   - RBTree    : orden cronologico y rangos      O(log n)
// El sistema es DUENO de la memoria de cada Pedido (new en registro/carga,
// delete al cancelar o en el destructor). Las estructuras solo guardan punteros.
class SistemaPedidos {
private:
    HashTable hash;
    HeapArray heap;
    RBTree    arbol;
    int       total;
    double    montoTotal;
    string    primerId;     // primer y ultimo ID cargados desde archivo
    string    ultimoId;

    // Inserta en las 3 estructuras; si 'tc' != nullptr acumula tiempos.
    bool insertarEnEstructuras(Pedido* p, TiemposCarga* tc);

public:
    SistemaPedidos();
    ~SistemaPedidos();
    SistemaPedidos(const SistemaPedidos&) = delete;
    SistemaPedidos& operator=(const SistemaPedidos&) = delete;

    // 1. Carga masiva desde .csv/.txt
    bool cargarCsv(const string& ruta, TiemposCarga& tc);

    // 2. Registro manual. false si el ID ya existe (p NO se toma en ese caso:
    //    el llamador debe hacer delete). true: el sistema pasa a ser dueno de p.
    bool registrar(Pedido* p);

    // 3. Busqueda por ID (Hash)
    Pedido* buscar(const string& id) const { return hash.buscar(id); }

    // 4. Despacha el pedido de mayor prioridad: lo saca del heap y lo pasa a EnTransito
    Pedido* despacharSiguiente();

    // 5. Recorrido in-order limitado (primeros 'limite')
    int consultaOrdenada(int limite, void (*visitar)(Pedido*, void*), void* ctx) const {
        return arbol.inOrder(limite, visitar, ctx);
    }

    // 6. Rango de fechas
    void consultaRango(const string& ini, const string& fin,
                       int& cantidad, double& monto, int& visitados) const {
        arbol.rango(ini, fin, cantidad, monto, visitados);
    }

    // 7. Actualizar estado. Mantiene el heap consistente
    //    (solo los 'Pendiente' esperan despacho).
    bool actualizarEstado(const string& id, const string& nuevoEstado);

    // 8. Cancelar: elimina de Hash, Arbol y Heap y libera memoria.
    bool cancelar(const string& id);

    // 9. Estadisticas en O(1)
    int    getTotal()      const { return total; }
    int    getPendientes() const { return heap.getTamano(); }
    double getMontoTotal() const { return montoTotal; }

    const string& getPrimerId() const { return primerId; }
    const string& getUltimoId() const { return ultimoId; }

    // Acceso de solo lectura a las estructuras (informe / diagnostico)
    const HashTable& getHash()  const { return hash; }
    const HeapArray& getHeap()  const { return heap; }
    const RBTree&    getArbol() const { return arbol; }

    // Copia los punteros de todos los pedidos (orden cronologico).
    // El llamador reserva getTotal() posiciones.
    int recolectar(Pedido** out) const { return arbol.recolectar(out); }

    // Verifica consistencia: mismo numero de pedidos en hash y arbol,
    // arbol RB valido, heap valido.
    bool verificarConsistencia() const;
};

#endif
