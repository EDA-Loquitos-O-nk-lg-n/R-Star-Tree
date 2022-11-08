#ifndef ENTRADA_H
#define ENTRADA_H

#include"Nodo.h"
#include"Intervalo.h"
#include"Punto.h"
#include<limits>
#include<algorithm>

struct Nodo;

struct Entrada{
    Entrada();
    ~Entrada();
    Entrada(Nodo* H);
    Entrada(const vector<Punto> &P);
    void actualizar_rectangulo();
    Nodo* puntero_hijo;
    Intervalo rectangulo[2];
    vector<Punto> objeto;

    int ampliacion(Entrada* K);
    int area();
    int perimetro();
    int superposicion(Entrada* K);
    inline Punto centro();
    float distancia_centros(Entrada* K);

    bool dentro(int x, int y);

    int lado_x();
    int lado_y();

};
bool comparar_rectangulo_menor_X(Entrada *e1, Entrada *e2);
bool comparar_rectangulo_menor_Y(Entrada *e1, Entrada *e2);
bool comparar_rectangulo_mayor_X(Entrada *e1, Entrada *e2);
bool comparar_rectangulo_mayor_Y(Entrada *e1, Entrada *e2);

#endif