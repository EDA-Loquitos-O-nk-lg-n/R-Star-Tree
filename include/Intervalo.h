#ifndef INTERVALO_H
#define INTERVALO_H

struct Intervalo{
    int menor, mayor;
    bool dentro(int p);
};

#endif