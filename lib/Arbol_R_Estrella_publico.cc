#include"../include/Arbol_R_Estrella.h"

int Arbol_R_Estrella::retornar_altura(){
    int altura = 0;
    Nodo* N = raiz;
    while(!N->hoja){
        N = N->entradas.front()->puntero_hijo;
        altura++;
    }
    return altura;
}

double Arbol_R_Estrella::obtener_sobrelapado_total(){
    double area_total=0, area_sobrelapada=0;

    set<int> seg_set_x, seg_set_y;

    queue<Nodo*> cola_nodos;
    cola_nodos.push(raiz);
    while(!cola_nodos.empty()){
        if(!cola_nodos.front()->hoja){
            for(Entrada* e: cola_nodos.front()->entradas){
                seg_set_x.insert(e->rectangulo[0].menor);
                seg_set_x.insert(e->rectangulo[0].mayor);
                seg_set_y.insert(e->rectangulo[1].menor);
                seg_set_y.insert(e->rectangulo[1].mayor);
                cola_nodos.push(e->puntero_hijo);
            }
        }
        cola_nodos.pop();
    }


    //////
    vector<int> segmento_x(seg_set_x.begin(), seg_set_x.end()), segmento_y(seg_set_y.begin(), seg_set_y.end());
    vector<vector<int>> matriz(seg_set_x.size() - 1, vector<int>(seg_set_y.size() - 1, 0));

    cola_nodos.push(raiz);
    for(int ix = 1; ix<segmento_x.size(); ix++){
        for(int iy = 1; iy<segmento_y.size(); iy++){
            
            while(!cola_nodos.empty()){
                if(!cola_nodos.front()->hoja){
                    for(Entrada* e: cola_nodos.front()->entradas){
                        if(e->dentro(segmento_x[ix-1], segmento_y[iy-1]) && e->dentro(segmento_x[ix], segmento_y[iy])){
                            matriz[ix-1][iy-1]++;
                        }
                    cola_nodos.push(e->puntero_hijo);
                    }
                }
                cola_nodos.pop();
            }
        }
    }
    //////

    for(int i = 0; i<segmento_x.size()-1; i++){
        for(int j = 0; j<segmento_y.size()-1; j++){
            if(matriz[i][j] > 1){
                area_sobrelapada += (segmento_x[i+1]-segmento_x[i])*(segmento_y[j+1]-segmento_y[j]);
            }
            area_total += (segmento_x[i+1]-segmento_x[i])*(segmento_y[j+1]-segmento_y[j]);
        }
    }

    return area_sobrelapada/area_total;
}
double Arbol_R_Estrella::obtener_sobrelapado(int Nivel){

    double area_total=0, area_sobrelapada=0;

    priority_queue<pair<int, Nodo*>, vector<pair<int, Nodo*>>, greater<pair<int,Nodo*>>> desciende;
    desciende.push({0,raiz});
    while(desciende.top().first + 1 < Nivel){
        Nodo* N = desciende.top().second;
        int nivel_N = desciende.top().first;
        desciende.pop();
        for(Entrada* e: N->entradas){
            desciende.push({nivel_N+1, e->puntero_hijo});
        }
    }

    vector<Nodo*> nodos;
    while(!desciende.empty()){
        // cout<<desciende.top().first<<endl;
        nodos.push_back(desciende.top().second);
        desciende.pop();
    }

    // cout<<"NIVEL: "<<Nivel<<endl;
    // cout<<"NUMERO DE NODOS: "<<nodos.size()<<endl;

    set<int> seg_set_x, seg_set_y;
    for(Nodo* n: nodos){
        for(Entrada* e: n->entradas){
            seg_set_x.insert(e->rectangulo[0].menor);
            seg_set_x.insert(e->rectangulo[0].mayor);
            seg_set_y.insert(e->rectangulo[1].menor);
            seg_set_y.insert(e->rectangulo[1].mayor);
        }
    }
    vector<int> segmento_x(seg_set_x.begin(), seg_set_x.end()), segmento_y(seg_set_y.begin(), seg_set_y.end());
    vector<vector<int>> matriz(seg_set_x.size() - 1, vector<int>(seg_set_y.size() - 1, 0));

    for(int ix = 1; ix<segmento_x.size(); ix++){
        for(int iy = 1; iy<segmento_y.size(); iy++){
            for(Nodo* n :nodos){
                for(Entrada* e: n->entradas){
                    if(e->dentro(segmento_x[ix-1], segmento_y[iy-1]) && e->dentro(segmento_x[ix], segmento_y[iy])){
                        matriz[ix-1][iy-1]++;
                    }
                }
            }
        }
    }

    for(int i = 0; i<segmento_x.size()-1; i++){
        for(int j = 0; j<segmento_y.size()-1; j++){
            if(matriz[i][j] > 1){
                area_sobrelapada += (segmento_x[i+1]-segmento_x[i])*(segmento_y[j+1]-segmento_y[j]);
            }
            area_total += (segmento_x[i+1]-segmento_x[i])*(segmento_y[j+1]-segmento_y[j]);
        }
    }

    return area_sobrelapada/area_total;

}

bool Arbol_R_Estrella::buscar_exacto(const vector<Punto>& Ps){
    Entrada verificadora{Ps};

    queue<Nodo*> cola;
    cola.push(raiz);
    while(!cola.empty()){
        if(cola.front()->hoja){
            for(Entrada* e: cola.front()->entradas){
                if(Ps.size() != e->objeto.size()){
                    continue;
                }

                bool iguales = true;
                for (int i_punto = 0; i_punto < Ps.size(); i_punto++){
                    if(Ps[i_punto].x != e->objeto[i_punto].x || Ps[i_punto].y != e->objeto[i_punto].y){
                        iguales = false;
                        break;
                    }
                }
                if(iguales){
                    return true;
                }                
            }
        }
        else{
            for(Entrada* e: cola.front()->entradas){
                if(!e->dentro(verificadora.rectangulo[0].menor, verificadora.rectangulo[1].menor) 
                    || !e->dentro(verificadora.rectangulo[0].mayor, verificadora.rectangulo[1].mayor)){
                        continue;
                    }
                cola.push(e->puntero_hijo);
            }
        }
        cola.pop();
    }
    return false;
}

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
    objetos--;
    // imprimir_cantidad_nodos();
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
    objetos++;
    // imprimir_cantidad_nodos();
} 

