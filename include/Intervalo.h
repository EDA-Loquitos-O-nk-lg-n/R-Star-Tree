#ifndef INTERVALO_H
#define INTERVALO_H

// Estrcutura intervalo [menor, mayor]
struct Intervalo{
    // Cota inferior, cota superior
    int menor, mayor;
    // Determinar si un valor está en el intervalo
    bool dentro(int p);
};

#endif