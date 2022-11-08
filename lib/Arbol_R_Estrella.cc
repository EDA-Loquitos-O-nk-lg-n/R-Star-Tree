#include"../include/Arbol_R_Estrella.h"

void Arbol_R_Estrella::obtener_altura(){
    altura = 0;
    Nodo* N = raiz;
    while(!N->hoja){
        N = N->entradas.front()->puntero_hijo;
        altura++;
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
        for(int j = 0; i<segmento_y.size()-1; i++){
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
        for(int j = 0; i<segmento_y.size()-1; i++){
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

Arbol_R_Estrella::Arbol_R_Estrella(): raiz(new Nodo{true, nullptr}){
    obtener_altura();
}

Arbol_R_Estrella::~Arbol_R_Estrella(){
    delete raiz;
}

// Algorithm InsertData
void Arbol_R_Estrella::insertar(const vector<Punto>& R){
    Entrada* Entrada_R = new Entrada{R};
    insercion(Entrada_R, true);
} 

// Algorithm Insert
void Arbol_R_Estrella::insercion(Entrada* R, bool F){
    // I1
    Nodo* Apropiado{escoger_subarbol(F, R)};

    // I2
    Apropiado->entradas.push_back(R);
    if(Apropiado->entradas.size() >= Constante::M){
        tratar_desborde(Apropiado, F);
    }

    // I3

}

// Algorithm ChooseSubtree
Nodo* Arbol_R_Estrella::escoger_subarbol(int Nivel,  Entrada* ER){
    // CS1
    Nodo* N{raiz};

    // CS2
    while(!N->hoja){
        // Si las entradas son hojas
        if(N->entradas.front()->puntero_hijo == nullptr){
            // {puntero_entrada, incremento superposicion, area_ampliada, area}
            tuple<Entrada*, int, int, int> escoger{nullptr, numeric_limits<int>::max(), numeric_limits<int>::max(), numeric_limits<int>::max()};
            for(Entrada* ek: N->entradas){
                int superposicion = 0, nueva_superposicion = 0, ek_area = ek->area(), ek_area_ampliada = ek->ampliacion(ER);
                Entrada ek_ampliado;
                ek_ampliado.rectangulo[0] = {min(ek->rectangulo[0].menor, ER->rectangulo[0].menor), max(ek->rectangulo[0].mayor, ER->rectangulo[0].mayor)};
                ek_ampliado.rectangulo[1] = {min(ek->rectangulo[1].menor, ER->rectangulo[1].menor), max(ek->rectangulo[1].mayor, ER->rectangulo[1].mayor)};
                for(Entrada* er: N->entradas){
                    if(ek == er)
                        continue;

                    superposicion += ek->superposicion(er);
                    nueva_superposicion += ek_ampliado.superposicion(er);
                }

                if(nueva_superposicion - superposicion < get<1>(escoger)){
                    escoger = {ek, nueva_superposicion - superposicion, ek_area_ampliada, ek_area};
                }
                else if(nueva_superposicion - superposicion == get<1>(escoger)){
                    if(ek_area_ampliada < get<2>(escoger)){
                        escoger = {ek, nueva_superposicion - superposicion, ek_area_ampliada, ek_area};
                    }
                    else if(ek_area_ampliada == get<2>(escoger)){
                        if(ek_area < get<3>(escoger)){
                            escoger = {ek, nueva_superposicion - superposicion, ek_area_ampliada, ek_area};
                        }
                    }
                }
            }
            N = get<0>(escoger)->puntero_hijo;
        }
        else{
            tuple<Entrada*, int, int> escoger{nullptr, numeric_limits<int>::max(), numeric_limits<int>::max()};
            for(Entrada* ei: N->entradas){
                int ei_ampliacion{ei->ampliacion(ER)};
                int ei_area{ei->area()};

                if(ei_ampliacion < get<1>(escoger)){
                    escoger = {ei, ei_ampliacion, ei_area};
                }
                else if(ei_ampliacion == get<1>(escoger)){
                    if(ei_area < get<2>(escoger)){
                        escoger = {ei, ei_ampliacion, ei_area};
                    }
                }
            }
            N = get<0>(escoger)->puntero_hijo;
        }
    }

    return N;
}

// Alogrithm Split
Nodo* Arbol_R_Estrella::dividir(Nodo* N){

    // S1
    int eje{escoger_eje_division(N)};

    // S2
    int indice_k{escoger_indice_division(N, eje)};

    // S3
    Nodo* N_partido = new Nodo{N->hoja};
    N->entradas.resize(Constante::m - 1 + indice_k);
    N_partido->entradas = vector<Entrada*>(next(N->entradas.begin(), Constante::m - 1 + indice_k), N->entradas.end());

    return N_partido;
}

// Algorithm ChooseSpliAxis
int Arbol_R_Estrella::escoger_eje_division(Nodo* N){
    // CSA1
    Intervalo k_intervalo{1, Constante::M - 2*Constante::m + 2};
    // [eje x:0, eje y:1][orden_menor:0, orden_mayor:1 ]
    // vector<int> S[2]{{k.mayor-k.menor + 1, 0}, {k.mayor-k.menor + 1, 0}};
    int S[2]{};

    vector<Entrada*> entradas = N->entradas;

    Entrada et1, et2; // entrada_temporal
    Nodo nt1, nt2; // nodo_temporal
    et1.puntero_hijo = &nt1;
    et2.puntero_hijo = &nt2;

    sort(entradas.begin(), entradas.end(), comparar_rectangulo_menor_X);
    S[0] += calcular_margen(k_intervalo, entradas, et1, et2);
    sort(entradas.begin(), entradas.end(), comparar_rectangulo_mayor_X);
    S[0] += calcular_margen(k_intervalo, entradas, et1, et2);
    sort(entradas.begin(), entradas.end(), comparar_rectangulo_menor_Y);
    S[1] += calcular_margen(k_intervalo, entradas, et1, et2);
    sort(entradas.begin(), entradas.end(), comparar_rectangulo_mayor_Y);
    S[1] += calcular_margen(k_intervalo, entradas, et1, et2);

    // CSA2
    return S[0]<S[1];
}

// Algorithm ChooseSplitIndex
int Arbol_R_Estrella::escoger_indice_division(Nodo* N, int eje){
    Intervalo I{1, Constante::M - 2*Constante::m + 2};
    // indice-k, sobrelapar, area
    tuple<int, int, int> criterio{0,numeric_limits<int>::max(),numeric_limits<int>::max()};

    for(int k = I.menor; k<= I.mayor; k++){
        pair<int, int> evaluador = calcular_sobrelapamiento(N, Constante::m - 1 + k);
        if(evaluador.first < get<1>(criterio)){
            get<0>(criterio) = k;
            get<1>(criterio) = evaluador.first;
            get<2>(criterio) = evaluador.second;
        }
        else if(evaluador.first == get<1>(criterio) && evaluador.second < get<2>(criterio)){
            get<0>(criterio) = k;
            get<1>(criterio) = evaluador.first;
            get<2>(criterio) = evaluador.second;
        }
    }

    return get<0>(criterio);
}

// Algorithm OverflowTreatment
Nodo* Arbol_R_Estrella::tratar_desborde(Nodo* N, bool F){
    if(F && N != raiz){
        reinsertar(N);
    }
    else{
        dividir(N);
    }
    return nullptr;
}

// Algorithm ReInsert
void Arbol_R_Estrella::reinsertar(Nodo* N){
    // RI1
    Entrada* e = nullptr;
    for(Entrada* et: N->padre->entradas){
        if(et->puntero_hijo == N){
            e = et;
            break;
        }
    }

    // RI2
    priority_queue<pair<float, Entrada*>, deque<pair<float, Entrada*>>, greater<pair<float, Entrada*>>> distancias;
    for(Entrada* et: N->entradas){
        distancias.push({e->distancia_centros(et), et});
    }
    N->entradas.clear();

    // RI3
    int conservar = distancias.size() - (distancias.size() * 3) / 10;
    for(int i = 0; i<conservar; i++){
        N->entradas.push_back(distancias.top().second);
        distancias.pop();
    }
    e->actualizar_rectangulo();

    // RI4
    while(!distancias.empty()){
        insercion(distancias.top().second);
        distancias.pop();
    }
}