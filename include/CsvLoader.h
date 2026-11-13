#ifndef CSVLOADER_H
#define CSVLOADER_H

#include "Pedido.h"
#include <string>
using namespace std;

Pedido* parsearPedido(const string& linea);

#endif
