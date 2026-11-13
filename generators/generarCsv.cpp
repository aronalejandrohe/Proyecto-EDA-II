#include "generarCsv.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
using namespace std;

string tiposEnvio[] = {"Normal", "Express", "SameDay"};

string generarDni() {
    string dni = "";
    for (int i = 0; i < 8; i++) dni += char('0' + rand() % 10);
    return dni;
}

string dosDigitos(int n) {
    return (n < 10 ? "0" : "") + to_string(n);
}

string generarFecha() {
    int mes = rand() % 12 + 1;
    int dia = rand() % 28 + 1;
    return "2026-" + dosDigitos(mes) + "-" + dosDigitos(dia);
}

string generarHora() {
    int h = rand() % 24;
    int m = rand() % 60;
    int s = rand() % 60;
    return dosDigitos(h) + ":" + dosDigitos(m) + ":" + dosDigitos(s);
}

void generarArchivo(const string& ruta, int cantidad) {
    ofstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo crear " << ruta << endl;
        return;
    }
    archivo << fixed << setprecision(2);
    for (int i = 1; i <= cantidad; i++) {
        string numero = to_string(i);
        while (numero.size() < 6) numero = "0" + numero;
        string id = "PD-" + numero;
        string tipo = tiposEnvio[rand() % 3];
        int prioridad = rand() % 4 + 1;
        if (tipo == "Express") prioridad = rand() % 3 + 5;
        if (tipo == "SameDay") prioridad = rand() % 3 + 8;
        double monto = (rand() % 199500 + 500) / 100.0;
        archivo << id << ',' << generarDni() << ',' << tipo << ','
                << prioridad << ',' << monto << ',' << generarFecha() << ','
                << generarHora() << ",Pendiente\n";
    }
    cout << "Generado: " << ruta << " (" << cantidad << " pedidos)" << endl;
}

void generarArchivos() {
    srand(2026);
    generarArchivo("data/pedidos_pequeno.csv", 30);
    generarArchivo("data/pedidos_masivo.csv", 10000);
}
