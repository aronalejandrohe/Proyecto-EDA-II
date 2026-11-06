#include "../include/Pedido.h"
#include <iostream>
#include <iomanip>
using namespace std;

Pedido::Pedido() {
    nivelPrioridad = 0;
    montoDolares   = 0.0;
}

Pedido::Pedido(string id, string dni, string tipo, int prioridad,
               double monto, string f, string h, string est) {
    idPedido       = id;
    dniCliente     = dni;
    tipoEnvio      = tipo;
    nivelPrioridad = prioridad;
    montoDolares   = monto;
    fecha          = f;
    hora           = h;
    estado         = est;
}

string Pedido::getFechaHora() const {
    return fecha + " " + hora;
}

void Pedido::imprimir() const {
    cout << "ID: "         << idPedido
         << " | DNI: "     << dniCliente
         << " | Tipo: "    << tipoEnvio
         << " | Prior: "   << nivelPrioridad
         << " | Monto: $"  << fixed << setprecision(2) << montoDolares
         << " | Fecha: "   << fecha
         << " | Hora: "    << hora
         << " | Estado: "  << estado
         << endl;
}
