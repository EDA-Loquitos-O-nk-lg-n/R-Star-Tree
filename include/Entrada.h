#ifndef ENTRADA_H
#define ENTRADA_H

#include"Nodo.h"
#include"Intervalo.h"
#include"Punto.h"
#include<limits>
#include<algorithm>

struct Nodo;

// Estrucutra de la entrada
struct Entrada{
    // Constructor
    Entrada();
    // Destructor
    ~Entrada();
    // Constructor para entrada interna
    Entrada(Nodo* H);
    // Constructor para entrada hoja
    Entrada(const vector<Punto> &P);
    // Actualizar los intervalos
    void actualizar_rectangulo();
    // puntero a hijo en caso sea interno
    Nodo* puntero_hijo;
    // Limites de MBR
    Intervalo rectangulo[2];
    // vector de puntos en caso sea hoja
    vector<Punto> objeto;

    // Calcular ampliación si K fuese añadida
    int ampliacion(Entrada* K);
    // Area del MBR
    int area();
    // Perimetro del MBR
    int perimetro();
    // Calcular el solapamiento de la actual con la entrada K
    int superposicion(Entrada* K);
    // Hallar el punto central del MBR
    inline Punto centro();
    // Distancia a centors
    float distancia_centros(Entrada* K);

    //Determinar si un punto está dentro del MBR
    bool dentro(int x, int y);

    // Disatncia x del MBR
    int lado_x();
    // Disatncia y  del MBR
    int lado_y();

};
// Comparar de acuerdo a la coat inferior de x
bool comparar_rectangulo_menor_X(Entrada *e1, Entrada *e2);
// Comparar de acuerdo a la coat inferior de y
bool comparar_rectangulo_menor_Y(Entrada *e1, Entrada *e2);
// Comparar de acuerdo a la coat superior de x
bool comparar_rectangulo_mayor_X(Entrada *e1, Entrada *e2);
// Comparar de acuerdo a la coat superior de y
bool comparar_rectangulo_mayor_Y(Entrada *e1, Entrada *e2);

#endif