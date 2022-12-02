#ifndef PUNTO_H
#define PUNTO_H

#include<cmath>

// Resumir una coordenada
struct Punto{
    // valor en x e valor en y
    int x, y;
    // Obtener la distancia euclidiana de un punto a otro
    float distancia(Punto p);
};

#endif