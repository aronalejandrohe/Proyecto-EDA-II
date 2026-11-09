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

// Compara dos pedidos cronologicamente: fecha, luego hora, luego idPedido
// (el idPedido desempata para que la clave del arbol sea unica).
// Devuelve <0, 0 o >0.
int compararCronologico(const Pedido& a, const Pedido& b);

// Validaciones de formato
bool fechaValida(const string& f);   // YYYY-MM-DD real (con bisiestos)
bool horaValida(const string& h);    // HH:MM:SS
bool estadoValido(const string& e);  // uno de los estados permitidos

// Estados permitidos por el sistema
extern const char* const estados_validos[];
extern const int num_estados;

#endif
