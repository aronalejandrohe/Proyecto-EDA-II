#include "../include/CsvLoader.h"
#include "../include/Pedido.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// Separa una linea CSV por comas y devuelve los campos
vector<string> splitCsv(const string& linea) {
    vector<string> campos;
    stringstream ss(linea);
    string campo;
    while (getline(ss, campo, ','))
        campos.push_back(campo);
    return campos;
}

vector<Pedido*> cargarCsv(const string& ruta) {
    vector<Pedido*> pedidos;
    ifstream archivo(ruta.c_str());

    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir " << ruta << endl;
        return pedidos;
    }

    string linea;
    int lineaNum = 0;

    while (getline(archivo, linea)) {
        lineaNum++;

        // Saltar lineas vacias
        if (linea.empty()) continue;

        vector<string> campos = splitCsv(linea);

        // Verificar que tenga los 8 campos
        if (campos.size() != 8) {
            cout << "ADVERTENCIA: linea " << lineaNum
                 << " tiene " << campos.size()
                 << " campos, se esperaban 8. Saltando..." << endl;
            continue;
        }

        // Crear el pedido en el heap
        Pedido* p = new Pedido(
            campos[0],           // idPedido
            campos[1],           // dniCliente
            campos[2],           // tipoEnvio
            atoi(campos[3].c_str()),   // nivelPrioridad
            atof(campos[4].c_str()),   // montoDolares
            campos[5],           // fecha
            campos[6],           // hora
            campos[7]            // estado
        );

        pedidos.push_back(p);
    }

    archivo.close();
    cout << "CSV cargado: " << pedidos.size()
         << " pedidos desde " << ruta << endl;

    return pedidos;
}
