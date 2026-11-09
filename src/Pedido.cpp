#include "../include/Pedido.h"
#include <iostream>
#include <iomanip>
using namespace std;

const char* const estados_validos[] = {
    "Pendiente", "Iniciado", "Enviado", "EnTransito", "Entregado", "Cancelado"
};
const int num_estados = 6;

Pedido::Pedido() {
    nivelPrioridad = 0;
    montoDolares   = 0.0;
    heapIdx        = -1;
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
    heapIdx        = -1;
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

int compararCronologico(const Pedido& a, const Pedido& b) {
    int c = a.fecha.compare(b.fecha);
    if (c != 0) return c;
    c = a.hora.compare(b.hora);
    if (c != 0) return c;
    return a.idPedido.compare(b.idPedido);
}

static bool soloDigitos(const string& s, int desde, int hasta) {
    for (int i = desde; i < hasta; i++)
        if (s[i] < '0' || s[i] > '9') return false;
    return true;
}

bool fechaValida(const string& f) {
    if (f.size() != 10 || f[4] != '-' || f[7] != '-') return false;
    if (!soloDigitos(f, 0, 4) || !soloDigitos(f, 5, 7) || !soloDigitos(f, 8, 10))
        return false;
    int anio = atoi(f.substr(0, 4).c_str());
    int mes  = atoi(f.substr(5, 2).c_str());
    int dia  = atoi(f.substr(8, 2).c_str());
    if (mes < 1 || mes > 12 || dia < 1) return false;
    int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
    int maxDia = dias[mes - 1] + ((mes == 2 && bisiesto) ? 1 : 0);
    return dia <= maxDia;
}

bool horaValida(const string& h) {
    if (h.size() != 8 || h[2] != ':' || h[5] != ':') return false;
    if (!soloDigitos(h, 0, 2) || !soloDigitos(h, 3, 5) || !soloDigitos(h, 6, 8))
        return false;
    int hh = atoi(h.substr(0, 2).c_str());
    int mm = atoi(h.substr(3, 2).c_str());
    int ss = atoi(h.substr(6, 2).c_str());
    return hh < 24 && mm < 60 && ss < 60;
}

bool estadoValido(const string& e) {
    for (int i = 0; i < num_estados; i++)
        if (e == estados_validos[i]) return true;
    return false;
}
