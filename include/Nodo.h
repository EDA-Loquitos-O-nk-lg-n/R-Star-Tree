#ifndef NODO_H
#define NODO_H

#include<vector>

using namespace std;

struct Entrada;

struct Nodo{
    bool hoja;
    Nodo* padre;
    vector<Entrada*> entradas;
};

#endif