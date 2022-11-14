#include"../include/Arbol_R_Estrella.h"

void Arbol_R_Estrella::eliminar(Punto P){
    if(raiz->entradas.size() == 0){
        return;
    }
    // D1
    Distante cerca = buscar(P, 1)[0];
    Entrada* E = cerca.entrada;
    Nodo* L = cerca.nodo;

    // D2
    for(int i = 0; i<L->entradas.size(); i++){
        if(L->entradas[i] == E){
            L->entradas.erase(next(L->entradas.begin(), i));
            break;
        }
    }

    // D3
    condensar_cercano(L);

    // D4
    if(raiz->entradas.size() == 1 && !raiz->hoja){
        Nodo* r{raiz};
        raiz = raiz->entradas.front()->puntero_hijo;
        raiz->padre = nullptr;
        delete r->entradas.front(), r;
    }

    obtener_altura();

    imprimir_cantidad_nodos();
}

vector<Arbol_R_Estrella::Distante> Arbol_R_Estrella::buscar(Punto R, int k){
    priority_queue<Arbol_R_Estrella::Distante, deque<Arbol_R_Estrella::Distante>, greater<Arbol_R_Estrella::Distante>> knn_lista;
    for(int i = 0; i<raiz->entradas.size(); i++){
        knn_lista.push({raiz->entradas[i], R, raiz});
    }

    vector<Arbol_R_Estrella::Distante> resultados;
    while(resultados.size() < k && !knn_lista.empty()){
        if(knn_lista.top().nodo->hoja){
            resultados.push_back(knn_lista.top());
            knn_lista.pop();
        }
        else{
            Entrada* ET = knn_lista.top().entrada;
            knn_lista.pop();
            for(int i = 0; i<ET->puntero_hijo->entradas.size(); i++)
                knn_lista.push({ET->puntero_hijo->entradas[i], R, ET->puntero_hijo});
        }
    }
    return resultados;
}

Arbol_R_Estrella::Arbol_R_Estrella(): raiz(new Nodo{true, nullptr}){
    obtener_altura();
}

Arbol_R_Estrella::~Arbol_R_Estrella(){
    destruir_recursivo(raiz);
}

// Algorithm InsertData
void Arbol_R_Estrella::insertar(const vector<Punto>& R){
    Entrada* Entrada_R = new Entrada{R};
    insercion(Entrada_R, altura, true);
    obtener_altura();

    imprimir_cantidad_nodos();
} 

