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
    // Constructor
    Arbol_R_Estrella();
    // Destructor
    ~Arbol_R_Estrella();
    // Algorithm InsertData
    void insertar(const vector<Punto>& R); 
    
    // ALgoritmo eleiminación (prestado del arbol_R)
    void eliminar(Punto R);

    // Estrctura de objeto KNN
    struct Distante{
        // Constructo
        Distante(Entrada* E, Punto P, Nodo *N);
        // Destructor
        ~Distante();
        // Entrada referenciada
        Entrada* entrada;
        // Distancia de P al centro de la entrada E
        double distancia;
        // Nodo referenciada
        Nodo* nodo;
    };
    // Busqueda KNN
    vector<Distante> buscar(Punto R, int k);
    // Busqeuda exacta
    bool buscar_exacto(const vector<Punto>& Ps);
    // OBtener la altura
    int retornar_altura();
    // Sobrelapado por nivel del arbol
    double obtener_sobrelapado(int Nivel);
    // Sobrelapado de todos los niveles
    double obtener_sobrelapado_total();
protected:
    friend bool operator<(const Distante &pd1, const Distante &pd2);
    friend bool operator>(const Distante &pd1, const Distante &pd2);
    friend bool operator<=(const Distante &pd1, const Distante &pd2);
    friend bool operator>=(const Distante &pd1, const Distante &pd2);
    // Actualizar nodos superiores en eliminación
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
    // FUNCION DEPRECADA
    void imprimir_cantidad_nodos();
    // Destruir memoria dinamica
    void destruir_recursivo(Nodo* raiz);
    // Calcular altura atributo
    void obtener_altura();
    // Calcular el margen
    static int calcular_margen(Intervalo& I, vector<Entrada*>& vE, Entrada& et1, Entrada& et2);
    // Calcular el solapamiento
    static pair<int, int> calcular_sobrelapamiento(Nodo *N, int indice);
    // Nodo puntero raiz
    Nodo* raiz;
    // altura
    int altura;
    // cantidad de objetos
    int objetos;

    friend class Interfaz;
    // DEPRECADA
    friend void verificarPadres(Arbol_R_Estrella* a);
};

#endif