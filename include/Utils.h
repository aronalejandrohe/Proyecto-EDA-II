#ifndef UTILS_H
#define UTILS_H

#include <chrono>

class Cronometro {
private:
    std::chrono::steady_clock::time_point t0;
public:
    Cronometro() { iniciar(); }
    void iniciar() { t0 = std::chrono::steady_clock::now(); }
    double ms() const {
        return std::chrono::duration<double, std::milli>(
                   std::chrono::steady_clock::now() - t0).count();
    }
};

#endif
