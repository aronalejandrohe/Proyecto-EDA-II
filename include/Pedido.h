#ifndef PEDIDO_H
#define PEDIDO_H

#include <string>
using namespace std;

struct Pedido {
    string idPedido;
    string dniCliente;
    string tipoEnvio;
    int    nivelPrioridad;
    double montoDolares;
    string fecha;
    string hora;
    string estado;

    int    heapIdx;

    Pedido();

    Pedido(string id, string dni, string tipo, int prioridad,
           double monto, string fecha, string hora, string estado);

    void imprimir() const;
};

int compararCronologico(const Pedido& a, const Pedido& b);

bool fechaValida(const string& f);
bool horaValida(const string& h);
bool estadoValido(const string& e);
bool pedidoValido(const Pedido& p);

#endif
