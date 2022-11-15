#include"../include/Arbol_R_Estrella.h"

void Arbol_R_Estrella::obtener_altura(){
    altura = 0;
    Nodo* N = raiz;
    while(!N->hoja){
        N = N->entradas.front()->puntero_hijo;
        altura++;
    }
}

void Arbol_R_Estrella::imprimir_cantidad_nodos(){
    vector<int> entradas_por_nivel(altura+1, 0);
    function<void(Nodo*, int)> impresion = [&entradas_por_nivel, &impresion](Nodo* nodo, int nivel){
        for(Entrada* e: nodo->entradas){
            entradas_por_nivel[nivel]++;
            if(e->puntero_hijo != nullptr){
                impresion(e->puntero_hijo, nivel+1);
            }
        }
    };
    impresion(raiz, 0);
    for(int i = 0; i<entradas_por_nivel.size(); i++){
        cout<<"Nivel"<<i<<": "<<entradas_por_nivel[i]<<endl;
    }
}


pair<int, int> Arbol_R_Estrella::calcular_sobrelapamiento(Nodo *N, int indice){
    int area_sobrelapada = 0;
    int area_distribucion = 0;

    // MBR 1
    set<int> seg_set_x, seg_set_y;

    for(int i = 0; i<indice; i++){
        seg_set_x.insert(N->entradas[i]->rectangulo[0].menor);
        seg_set_y.insert(N->entradas[i]->rectangulo[0].mayor);
        seg_set_x.insert(N->entradas[i]->rectangulo[1].menor);
        seg_set_y.insert(N->entradas[i]->rectangulo[1].mayor);
    }

    vector<int> segmento_x(seg_set_x.begin(), seg_set_x.end()), segmento_y(seg_set_y.begin(), seg_set_y.end());
    vector<vector<int>> matriz(seg_set_x.size() - 1, vector<int>(seg_set_y.size() - 1, 0));
    
    for(int ix = 1; ix<segmento_x.size(); ix++){
        for(int iy = 1; iy<segmento_y.size(); iy++){
            for(int ie = 0; ie<indice; ie++){
                if(N->entradas[ie]->dentro(segmento_x[ix-1], segmento_y[iy-1]) && N->entradas[ie]->dentro(segmento_x[ix], segmento_y[iy])){
                    matriz[ix-1][iy-1]++;
                }
            }
        }
    }

    for(int i = 0; i<segmento_x.size()-1; i++){
        for(int j = 0; j<segmento_y.size()-1; j++){
            if(matriz[i][j] > 1){
                area_sobrelapada += (segmento_x[i+1]-segmento_x[i])*(segmento_y[j+1]-segmento_y[j]);
            }
        }
    }
    area_distribucion += (segmento_x.back() - segmento_x.front())*(segmento_y.back() - segmento_y.front());

    seg_set_x.clear();
    seg_set_y.clear();
    
    // MBR 2
    for(int i = indice; i<N->entradas.size(); i++){
        seg_set_x.insert(N->entradas[i]->rectangulo[0].menor);
        seg_set_x.insert(N->entradas[i]->rectangulo[0].mayor);
        seg_set_y.insert(N->entradas[i]->rectangulo[1].menor);
        seg_set_y.insert(N->entradas[i]->rectangulo[1].mayor);
    }
    segmento_x = vector<int>(seg_set_x.begin(), seg_set_x.end()); 
    segmento_y = vector<int>(seg_set_y.begin(), seg_set_y.end());
    matriz = vector<vector<int>>(seg_set_x.size() - 1, vector<int>(seg_set_y.size() - 1, 0));

    for(int ix = 1; ix<segmento_x.size(); ix++){
        for(int iy = 1; iy<segmento_y.size(); iy++){
            for(int ie = indice; ie<N->entradas.size(); ie++){
                if(N->entradas[ie]->dentro(segmento_x[ix-1], segmento_y[iy-1]) && N->entradas[ie]->dentro(segmento_x[ix], segmento_y[iy])){
                    matriz[ix-1][iy-1]++;
                }
            }
        }
    }

    for(int i = 0; i<segmento_x.size()-1; i++){
        for(int j = 0; j<segmento_y.size()-1; j++){
            if(matriz[i][j] > 1){
                area_sobrelapada += (segmento_x[i+1]-segmento_x[i])*(segmento_y[j+1]-segmento_y[j]);
            }
        }
    }
    area_distribucion += (segmento_x.back() - segmento_x.front())*(segmento_y.back() - segmento_y.front());

    return {area_sobrelapada, area_distribucion};
}

int Arbol_R_Estrella::calcular_margen(Intervalo& I, vector<Entrada*>& vE, Entrada& et1, Entrada& et2){
    for(int k=I.menor; k<=I.mayor; k++){
       et1.puntero_hijo->entradas = vector<Entrada*>(vE.begin(), next(vE.begin(), (Constante::m-1)+k)); 
       et2.puntero_hijo->entradas = vector<Entrada*>(next(vE.begin(), (Constante::m-1)+k), vE.end()); 
       et1.actualizar_rectangulo();
       et2.actualizar_rectangulo();
    }
    return et1.perimetro() + et2.perimetro();
}

void Arbol_R_Estrella::destruir_recursivo(Nodo* N){
    if(!N->hoja){
        for(Entrada* e: N->entradas){
            destruir_recursivo(e->puntero_hijo);
            delete e;
        }
    }
    else{
        for(Entrada* e: N->entradas){
            delete e;
        }
    }
    delete N;
}