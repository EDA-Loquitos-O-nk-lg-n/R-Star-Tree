#ifndef ARBOL_R_ESTRELLA_H
#define ARBOL_R_ESTRELLA_H

#include"Nodo.h"
#include"Punto.h"
#include"Entrada.h"
#include"Constante.h"
#include<functional>
#include<cmath>
#include<iostream>
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
    
    void eliminar(Punto R);
    struct Distante{
        Distante(Entrada* E, Punto P, Nodo *N);
        ~Distante();
        Entrada* entrada;
        double distancia;
        Nodo* nodo;
    };
    vector<Distante> buscar(Punto R, int k);
    bool buscar_exacto(const vector<Punto>& Ps);
protected:
    friend bool operator<(const Distante &pd1, const Distante &pd2);
    friend bool operator>(const Distante &pd1, const Distante &pd2);
    friend bool operator<=(const Distante &pd1, const Distante &pd2);
    friend bool operator>=(const Distante &pd1, const Distante &pd2);
    void condensar_cercano(Nodo* L);
    // Algorithm Insert
    void insercion(Entrada* R, int Nivel, bool F);
    // Algorithm ChooseSubtree
    Nodo* escoger_subarbol(int Nivel, Entrada* ER); 
    // Alogrithm Split
    Nodo* dividir(Nodo* N); 
    // Algorithm ChooseSpliAxis
    int escoger_eje_division(Nodo* N); 
    // Algorithm ChooseSplitIndex
    pair<int,bool> escoger_indice_division(Nodo* N, int eje); 
    // Algorithm OverflowTreatment
    Nodo* tratar_desborde(Nodo* N, int Nivel, bool Primera_Llamada);
    // Algorithm ReInsert
    void reinsertar(Nodo* N, int Nivel);

private:
    void imprimir_cantidad_nodos();
    void destruir_recursivo(Nodo* raiz);
    void obtener_altura();
    static int calcular_margen(Intervalo& I, vector<Entrada*>& vE, Entrada& et1, Entrada& et2);
    static pair<int, int> calcular_sobrelapamiento(Nodo *N, int indice);
    Nodo* raiz;
    int altura;
    int objetos;

    friend class Interfaz;
    friend void verificarPadres(Arbol_R_Estrella* a);
};

#endif