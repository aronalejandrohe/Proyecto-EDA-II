#include "../include/CsvLoader.h"
#include <cstdlib>
using namespace std;

Pedido* parsearPedido(const string& linea) {
    if (linea.empty()) return nullptr;
    string campos[8];
    int campo = 0;
    for (int i = 0; i < (int)linea.size(); i++) {
        char c = linea[i];
        if (c == '\r') continue;
        if (c == ',') {
            if (campo == 7) return nullptr;
            campo++;
        } else {
            campos[campo] += c;
        }
    }
    if (campo != 7) return nullptr;

    char* fin = nullptr;
    long prioridad = strtol(campos[3].c_str(), &fin, 10);
    if (campos[3].empty() || *fin != '\0' || prioridad < 1 || prioridad > 10)
        return nullptr;

    fin = nullptr;
    double monto = strtod(campos[4].c_str(), &fin);
    if (campos[4].empty() || *fin != '\0') return nullptr;

    Pedido* p = new Pedido(campos[0], campos[1], campos[2], (int)prioridad,
                            monto, campos[5], campos[6], campos[7]);
    if (!pedidoValido(*p)) {
        delete p;
        return nullptr;
    }
    return p;
}
