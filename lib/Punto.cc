#include "../include/Punto.h"

float Punto::distancia(Punto p){
    return sqrt(pow(x-p.x,2)+pow(y-p.y,2));
}