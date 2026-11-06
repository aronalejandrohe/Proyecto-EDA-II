#include "generarCsv.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

string tiposEnvio[] = {"Normal", "Express", "SameDay"};
string estados[]    = {"Pendiente", "EnTransito", "Enviado", "Entregado", "Cancelado"};

string generarDni() {
    string dni = "";
    for (int i = 0; i < 8; i++)
        dni += to_string(rand() % 10);
    return dni;
}

string generarFecha() {
    int mes = rand() % 12 + 1;
    int dia = rand() % 28 + 1;
    char buf[11];
    sprintf(buf, "2026-%02d-%02d", mes, dia);
    return string(buf);
}

string generarHora() {
    int h = rand() % 24;
    int m = rand() % 60;
    int s = rand() % 60;
    char buf[9];
    sprintf(buf, "%02d:%02d:%02d", h, m, s);
    return string(buf);
}

double generarMonto() {
    return (rand() % 199500 + 500) / 100.0;
}

void generarArchivo(const string& ruta, int cantidad) {
    ofstream archivo(ruta.c_str());
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo crear " << ruta << endl;
        return;
    }
    for (int i = 1; i <= cantidad; i++) {
        char id[12];
        sprintf(id, "PD-%06d", i);
        string tipo   = tiposEnvio[rand() % 3];
        int prioridad = rand() % 10 + 1;
        if (tipo == "Express") prioridad = rand() % 5 + 5;
        if (tipo == "SameDay") prioridad = rand() % 3 + 8;
        string estado = estados[rand() % 5];
        double monto  = generarMonto();
        archivo << id             << ","
                << generarDni()   << ","
                << tipo           << ","
                << prioridad      << ","
                << monto          << ","
                << generarFecha() << ","
                << generarHora()  << ","
                << estado         << "\n";
    }
    archivo.close();
    cout << "Generado: " << ruta << " (" << cantidad << " pedidos)" << endl;
}

void generarArchivos() {
    srand(time(0));
    generarArchivo("data/pedidos_pequeno.csv", 30);
    generarArchivo("data/pedidos_masivo.csv", 10000);
    cout << "\nListo. Revisa la carpeta data/" << endl;
}
