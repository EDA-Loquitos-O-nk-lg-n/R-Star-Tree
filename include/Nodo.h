#ifndef NODO_H
#define NODO_H

#include<vector>

using namespace std;

// Declarar la clase Entrada
struct Entrada;

// Nodo del arbol
struct Nodo{
    // Determina si es hoja
    bool hoja;
    // Apunta al Nodo padre, (raiz = nullptr)
    Nodo* padre;
    // Almacen de entradas
    vector<Entrada*> entradas;
};

#endif