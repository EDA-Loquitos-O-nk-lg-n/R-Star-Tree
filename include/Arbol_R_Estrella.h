#ifndef ARBOL_R_ESTRELLA_H
#define ARBOL_R_ESTRELLA_H

#include"Nodo.h"
#include"Punto.h"
#include"Entrada.h"
#include"Constante.h"
#include<functional>
#include<vector>
#include<queue>
#include<tuple>
#include<set>

struct Nodo;
struct Punto;
struct Entrada;


class Arbol_R_Estrella{
public:
    Arbol_R_Estrella();
    ~Arbol_R_Estrella();
    // Algorithm InsertData
    void insertar(const vector<Punto>& R); 
protected:
    // Algorithm Insert
    void insercion(Entrada* R, bool F=false);
    // Algorithm ChooseSubtree
    Nodo* escoger_subarbol(int Nivel, Entrada* ER); 
    // Alogrithm Split
    Nodo* dividir(Nodo* N); 
    // Algorithm ChooseSpliAxis
    int escoger_eje_division(Nodo* N); 
    // Algorithm ChooseSplitIndex
    pair<int,bool> escoger_indice_division(Nodo* N, int eje); 
    // Algorithm OverflowTreatment
    Nodo* tratar_desborde(Nodo* N, bool F=false);
    // Algorithm ReInsert
    void reinsertar(Nodo* N);

private:
    void obtener_altura();
    static int calcular_margen(Intervalo& I, vector<Entrada*>& vE, Entrada& et1, Entrada& et2);
    static pair<int, int> calcular_sobrelapamiento(Nodo *N, int indice);
    Nodo* raiz;
    int altura;
};

#endif