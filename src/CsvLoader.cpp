#include "../include/CsvLoader.h"
#include <cstdlib>
using namespace std;

// Convierte una linea del CSV en un Pedido.
// Devuelve nullptr si la linea esta vacia o tiene datos invalidos.
Pedido* parsearPedido(const string& linea) {
    if (linea.empty()) return nullptr;

    // 1) Separar la linea por comas en 8 campos
    string campos[8];
    int n = 0;                       // cuantos campos llevamos
    string actual = "";              // el campo que estamos armando

    for (int i = 0; i < (int)linea.size(); i++) {
        char c = linea[i];
        if (c == '\r') continue;     // ignorar salto de linea de Windows
        if (c == ',') {
            if (n >= 7) return nullptr;      // demasiados campos
            campos[n] = actual;
            n++;
            actual = "";
        } else {
            actual += c;
        }
    }
    campos[n] = actual;              // el ultimo campo no termina en coma
    n++;

    if (n != 8) return nullptr;      // faltan campos

    // 2) Validar los datos
    if (campos[0] == "" || campos[1] == "") return nullptr;
    if (!fechaValida(campos[5]))  return nullptr;
    if (!horaValida(campos[6]))   return nullptr;
    if (!estadoValido(campos[7])) return nullptr;

    // 3) Crear el pedido
    return new Pedido(
        campos[0],                    // idPedido
        campos[1],                    // dniCliente
        campos[2],                    // tipoEnvio
        atoi(campos[3].c_str()),      // nivelPrioridad
        atof(campos[4].c_str()),      // montoDolares
        campos[5],                    // fecha
        campos[6],                    // hora
        campos[7]                     // estado
    );
}
