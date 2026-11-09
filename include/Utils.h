#ifndef UTILS_H
#define UTILS_H

#include <chrono>

// Cronometro de alta resolucion (nanosegundos) para medir tiempos de ejecucion.
// Se usa std::chrono solo para MEDIR tiempo, no para almacenar datos.
class Cronometro {
private:
    std::chrono::steady_clock::time_point t0;
public:
    Cronometro() { iniciar(); }
    void iniciar() { t0 = std::chrono::steady_clock::now(); }
    // milisegundos (con decimales) desde iniciar()
    double ms() const {
        return std::chrono::duration<double, std::milli>(
                   std::chrono::steady_clock::now() - t0).count();
    }
};

#endif
