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

    // Constructor vacio
    Pedido();

    // Constructor con todos los campos
    Pedido(string id, string dni, string tipo, int prioridad,
           double monto, string fecha, string hora, string estado);

    // Para comparar en el RBTree por fecha+hora
    string getFechaHora() const;

    // Para mostrar en terminal
    void imprimir() const;
};

#endif
