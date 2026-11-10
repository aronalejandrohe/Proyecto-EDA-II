#ifndef CSVLOADER_H
#define CSVLOADER_H

#include "Pedido.h"
#include <string>
using namespace std;

// Convierte una linea CSV en un Pedido (reservado con new).
// Formato: id,dni,tipoEnvio,prioridad,monto,fecha,hora,estado
// Devuelve nullptr si la linea esta vacia o es invalida (campos faltantes,
// numeros incorrectos, fecha/hora/estado no validos). El llamador libera.
Pedido* parsearPedido(const string& linea);

#endif
