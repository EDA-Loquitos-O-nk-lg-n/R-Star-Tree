#ifndef INTERVALO_H
#define INTERVALO_H

struct Intervalo{
    int menor, mayor;
    inline bool dentro(int p);
};

#endif