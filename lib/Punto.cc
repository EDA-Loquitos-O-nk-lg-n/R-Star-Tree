#include "../include/Punto.h"

// Calcular la distancia de un punto a otro
float Punto::distancia(Punto p){
    // Aplicamos pitagoras
    return sqrt(pow(x-p.x,2)+pow(y-p.y,2));
}