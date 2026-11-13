#include "../include/Menu.h"
#include "../include/SistemaPedidos.h"
#include "../include/Utils.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;

static string pedir(const string& mensaje) {
    cout << mensaje;
    string s;
    getline(cin, s);
    return s;
}

static int leerNumero(const string& mensaje, int defecto = -1) {
    string s = pedir(mensaje);
    if (s.empty()) return defecto;
    char* fin;
    long n = strtol(s.c_str(), &fin, 10);
    if (*fin != '\0' || n < 0 || n > 1000000) return -1;
    return (int)n;
}

static double leerMonto() {
    string s = pedir("Monto en dolares: ");
    char* fin;
    double n = strtod(s.c_str(), &fin);
    return s.empty() || *fin != '\0' || !isfinite(n) ? -1 : n;
}

static string buscarRuta(string ruta) {
    string original = ruta;
    for (int i = 0; i < 3; i++) {
        ifstream archivo(ruta.c_str());
        if (archivo.good()) return ruta;
        ruta = "../" + ruta;
    }
    return original;
}

static void contarPedido(Pedido*, void* dato) {
    int* cantidad = (int*)dato;
    (*cantidad)++;
}

static void mostrarPedido(Pedido* p, void*) {
    p->imprimir();
}

static void menu(const SistemaPedidos& s) {
    cout << "\n===== SISTEMA DE GESTION LOGISTICA =====\n"
         << "Pedidos: " << s.getTotal() << " | Pendientes: " << s.getPendientes() << '\n'
         << "1. Carga masiva desde CSV\n"
         << "2. Registrar nuevo pedido\n"
         << "3. Buscar pedido por ID (Hash)\n"
         << "4. Despachar siguiente lote (Max-Heap)\n"
         << "5. Consulta ordenada por fecha y hora (30)\n"
         << "6. Consulta por rango de fechas\n"
         << "7. Actualizar estado\n"
         << "8. Cancelar pedido\n"
         << "9. Estadisticas generales\n"
         << "0. Salir\n";
}

void ejecutarMenu() {
    SistemaPedidos sistema;
    cout << fixed << setprecision(6);
    while (cin) {
        menu(sistema);
        int opcion = leerNumero("Opcion: ");
        if (opcion == 0) break;

        if (opcion == 1) {
            string ruta = pedir("Archivo [data/pedidos_masivo.csv]: ");
            if (ruta.empty()) ruta = "data/pedidos_masivo.csv";
            TiemposCarga t;
            if (!sistema.cargarCsv(buscarRuta(ruta), t)) {
                cout << "No se pudo abrir el archivo.\n";
                continue;
            }
            cout << "Cargados: " << t.leidos << " | Duplicados: " << t.duplicados
                 << " | Invalidos: " << t.invalidos << '\n'
                 << "Parseo CSV: " << t.lecturaMs << " ms | Hash: " << t.hashMs
                 << " ms | Arbol: " << t.arbolMs << " ms | Heap: " << t.heapMs
                 << " ms\nTotal de carga: " << t.totalMs << " ms\n";
        }
        else if (opcion == 2) {
            Pedido* p = new Pedido;
            p->idPedido = pedir("ID: ");
            if (sistema.buscar(p->idPedido)) {
                cout << "El ID ya existe.\n";
                delete p;
                continue;
            }
            p->dniCliente = pedir("DNI (8 digitos): ");
            p->tipoEnvio = pedir("Tipo (Normal/Express/SameDay): ");
            p->nivelPrioridad = leerNumero("Prioridad (1 a 10): ");
            p->montoDolares = leerMonto();
            p->fecha = pedir("Fecha (YYYY-MM-DD): ");
            p->hora = pedir("Hora (HH:MM:SS): ");
            p->estado = "Pendiente";
            if (!pedidoValido(*p)) {
                cout << "Datos invalidos. No se registro.\n";
                delete p;
                continue;
            }
            Cronometro c;
            bool registrado = sistema.registrar(p);
            cout << (registrado ? "Registrado" : "No registrado")
                 << " en " << c.ms() << " ms\n";
            if (!registrado) delete p;
        }
        else if (opcion == 3) {
            string id = pedir("ID: ");
            Cronometro c;
            Pedido* p = sistema.buscar(id);
            double ms = c.ms();
            if (p) p->imprimir();
            else cout << "Pedido no encontrado.\n";
            cout << "Tiempo Hash: " << ms << " ms\n";
        }
        else if (opcion == 4) {
            int n = leerNumero("Cantidad a despachar [10]: ", 10);
            if (n < 1) { cout << "Cantidad invalida.\n"; continue; }
            double ms = 0;
            int hechos = 0;
            for (int i = 0; i < n; i++) {
                Cronometro c;
                Pedido* p = sistema.despacharSiguiente();
                ms += c.ms();
                if (!p) break;
                p->imprimir();
                hechos++;
            }
            cout << "Despachados: " << hechos << " | Tiempo Heap: " << ms << " ms\n";
        }
        else if (opcion == 5) {
            int cantidad = 0;
            Cronometro c;
            sistema.consultaOrdenada(30, contarPedido, &cantidad);
            double ms = c.ms();
            sistema.consultaOrdenada(30, mostrarPedido, nullptr);
            cout << "Mostrados: " << cantidad << " | Tiempo arbol: " << ms << " ms\n";
        }
        else if (opcion == 6) {
            string inicio = pedir("Fecha inicial (YYYY-MM-DD): ");
            string fin = pedir("Fecha final   (YYYY-MM-DD): ");
            if (!fechaValida(inicio) || !fechaValida(fin) || inicio > fin) {
                cout << "Rango invalido.\n";
                continue;
            }
            int cantidad = 0, visitados = 0;
            double monto = 0;
            Cronometro c;
            sistema.consultaRango(inicio, fin, cantidad, monto, visitados);
            double ms = c.ms();
            cout << "Pedidos: " << cantidad << " | Monto: $" << setprecision(2) << monto
                 << setprecision(6) << "\nNodos visitados: " << visitados
                 << " | Tiempo: " << ms << " ms\n";
        }
        else if (opcion == 7) {
            string id = pedir("ID: ");
            if (!sistema.buscar(id)) { cout << "Pedido no encontrado.\n"; continue; }
            cout << "Estados: Pendiente, Iniciado, Enviado, EnTransito, Entregado, Cancelado\n";
            string estado = pedir("Nuevo estado: ");
            Cronometro c;
            bool ok = sistema.actualizarEstado(id, estado);
            double ms = c.ms();
            cout << (ok ? "Actualizado" : "Estado invalido") << " | Tiempo: " << ms << " ms\n";
        }
        else if (opcion == 8) {
            string id = pedir("ID: ");
            if (!sistema.buscar(id)) { cout << "Pedido no encontrado.\n"; continue; }
            string confirmar = pedir("Confirmar eliminacion (s/n): ");
            if (confirmar != "s" && confirmar != "S")
                continue;
            Cronometro c;
            bool ok = sistema.cancelar(id);
            double ms = c.ms();
            cout << (ok ? "Pedido eliminado" : "No se pudo eliminar")
                 << " | Tiempo: " << ms << " ms\n";
        }
        else if (opcion == 9) {
            cout << "Total: " << sistema.getTotal()
                 << " | Pendientes: " << sistema.getPendientes()
                 << " | Monto acumulado: $" << setprecision(2) << sistema.getMontoTotal()
                 << setprecision(6) << '\n';

            string respuesta = pedir("Ver colisiones, altura y memoria? (s/n): ");
            if (respuesta == "s" || respuesta == "S") {
                HashStats h = sistema.getHash().estadisticas();
                cout << "Hash: " << h.colisiones << " colisiones, cadena maxima " << h.maxCadena
                     << ", capacidad " << h.capacidad << "\nArbol: altura "
                     << sistema.getArbol().altura() << ", RB valido: "
                     << (sistema.getArbol().esValidoRB() ? "si" : "no")
                     << "\nMemoria estimada: Hash " << sistema.getHash().memoriaBytes() / 1024
                     << " KB, Arbol " << sistema.getArbol().memoriaBytes() / 1024
                     << " KB, Heap " << sistema.getHeap().memoriaBytes() / 1024 << " KB\n";
            }
        }
        else cout << "Opcion invalida.\n";
    }
}
