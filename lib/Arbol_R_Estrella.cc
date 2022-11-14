#include"../include/Arbol_R_Estrella.h"

void Arbol_R_Estrella::eliminar(Punto P){
    // D1
    Arbol_R_Estrella::Distante cerca = buscar(P, 1)[0];
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
    if(raiz->entradas.size() == 1){
        raiz = raiz->entradas[0]->puntero_hijo;
        raiz->padre = nullptr;
    }
}

void Arbol_R_Estrella::condensar_cercano(Nodo* L){
    // CT1
    Nodo* N = L;
    vector<Nodo*> Q;
    // CT2
    while(N != raiz){
        Nodo* P = N->padre;
        int E_N;
        bool eliminado = false;
        for(int i = 0; i<P->entradas.size(); i++){
            if(P->entradas[i]->puntero_hijo == N){
                E_N = i;
                break;
            }
        }
        // CT3
        if(N->entradas.size() < Constante::m){
            P->entradas.erase(next(P->entradas.begin(), E_N));
            eliminado = true;
            Q.push_back(N);
        }
        // CT4
        if(!eliminado){
            P->entradas[E_N]->actualizar_rectangulo();
        }
        // CT5
        N = P;
    }
    // CT6
    for(int i = 0; i<Q.size(); i++){
        // insercion_recursiva(Q[i]);
    }
}

Arbol_R_Estrella::Distante::Distante(Entrada* E, Punto P, Nodo *N): entrada(E), nodo(N){
    if(!N->hoja){
        if(P.x >= E->rectangulo[0].menor && P.x <= E->rectangulo[0].mayor && P.y >= E->rectangulo[1].menor && P.y <= E->rectangulo[1].mayor)
            distancia = 0;
        else if(P.x >= E->rectangulo[0].menor && P.x <= E->rectangulo[0].mayor)
            distancia = min(abs(P.y-E->rectangulo[1].menor), abs(P.y-E->rectangulo[1].mayor));
        else if(P.y >= E->rectangulo[1].menor && P.y <= E->rectangulo[1].mayor)
            distancia = min(abs(P.x-E->rectangulo[0].menor), abs(P.x-E->rectangulo[0].mayor));
        else{
            distancia = min(
                sqrt(pow(P.y-E->rectangulo[1].menor,2)+pow(P.x-E->rectangulo[0].menor,2)), 
                min(
                    sqrt(pow(P.y-E->rectangulo[1].mayor,2)+pow(P.x-E->rectangulo[0].menor,2)), 
                    min(
                        sqrt(pow(P.y-E->rectangulo[1].menor,2)+pow(P.x-E->rectangulo[0].mayor,2)), 
                        sqrt(pow(P.y-E->rectangulo[1].mayor,2)+pow(P.x-E->rectangulo[0].mayor,2))))
                );
        }
    }
    else{
        if(E->objeto.size() == 1){
            distancia = sqrt(pow(P.x-E->objeto[0].x,2)+pow(P.y-E->objeto[0].y,2));
        }
        else{
            double pm_x=0, pm_y=0;
            for(auto p: E->objeto){
                pm_x+=p.x;
                pm_y+=p.y;
            }
            pm_x/=E->objeto.size();
            pm_y/=E->objeto.size();
            distancia = sqrt(pow(P.x-pm_x,2)+pow(P.y-pm_y,2));
        }
    }
}

Arbol_R_Estrella::Distante::~Distante(){}

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

Arbol_R_Estrella::Arbol_R_Estrella(): raiz(new Nodo{true, nullptr}){
    obtener_altura();
}

void Arbol_R_Estrella::destruir_recursivo(Nodo* N){
    if(N != nullptr && !N->hoja){
        for(Entrada* e: N->entradas){
            destruir_recursivo(e->puntero_hijo);
        }
        delete N;
    }
}

bool operator<(const Arbol_R_Estrella::Distante &pd1, const Arbol_R_Estrella::Distante &pd2){
    return pd1.distancia < pd2.distancia;
}
bool operator>(const Arbol_R_Estrella::Distante &pd1, const Arbol_R_Estrella::Distante &pd2){
    return pd1.distancia>pd2.distancia;
}
bool operator<=(const Arbol_R_Estrella::Distante &pd1, const Arbol_R_Estrella::Distante &pd2){
    return pd1.distancia<=pd2.distancia;
}
bool operator>=(const Arbol_R_Estrella::Distante &pd1, const Arbol_R_Estrella::Distante &pd2){
    return pd1.distancia>=pd2.distancia;
}

Arbol_R_Estrella::~Arbol_R_Estrella(){
    destruir_recursivo(raiz);
}

// Algorithm InsertData
void Arbol_R_Estrella::insertar(const vector<Punto>& R){
    Entrada* Entrada_R = new Entrada{R};
    insercion(Entrada_R, true);
    obtener_altura();
} 

// Algorithm Insert
void Arbol_R_Estrella::insercion(Entrada* R, bool F){
    // I1
    Nodo* Apropiado{escoger_subarbol(F, R)};
    // I2, I3, I4
    Nodo* partido;
    while(Apropiado != nullptr){
        if(R != nullptr){
            Apropiado->entradas.push_back(R);
            if(!Apropiado->hoja){
                R->puntero_hijo->padre = Apropiado;
            }
        }

        if(Apropiado->entradas.size() > Constante::M){
            partido = tratar_desborde(Apropiado, F);
            if(partido != nullptr && Apropiado == raiz){
                raiz = new Nodo{false, nullptr, {new Entrada{raiz}, new Entrada{partido}}};
                raiz->entradas[0]->puntero_hijo->padre = raiz;
                raiz->entradas[1]->puntero_hijo->padre = raiz;
                return;
            }

            R = (partido != nullptr?new Entrada{partido}:nullptr);
        }
        else{
            R = nullptr;
        }

        if(Apropiado != raiz){
            Entrada* E_Apropiado = *find_if(Apropiado->padre->entradas.begin(), Apropiado->padre->entradas.end(), [Apropiado](Entrada* e){return e->puntero_hijo == Apropiado;});
            E_Apropiado->actualizar_rectangulo();
        }
        
        Apropiado = Apropiado->padre;
    }
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

    //Hardcodear las funciones de comparacion [eje][menor/mmayor]
    function<bool(Entrada*, Entrada*)> comparadores[2][2]{
        {&comparar_rectangulo_menor_X, &comparar_rectangulo_mayor_X},
        {&comparar_rectangulo_menor_Y, &comparar_rectangulo_mayor_Y}
    };

    // S1
    int eje{escoger_eje_division(N)};

    // S2
    pair<int, bool> indice_k_orden{escoger_indice_division(N, eje)};
    sort(N->entradas.begin(), N->entradas.end(),  comparadores[eje][indice_k_orden.second]);

    // S3
    Nodo* N_partido = new Nodo{N->hoja};
    N_partido->entradas = vector<Entrada*>(next(N->entradas.begin(), Constante::m - 1 + indice_k_orden.first), N->entradas.end());
    N->entradas.resize(Constante::m - 1 + indice_k_orden.first);
    if(!N_partido->hoja){
        for(Entrada* e: N_partido->entradas){
            e->puntero_hijo->padre = N_partido;
        }
    }
    return N_partido;
}

// Algorithm ChooseSpliAxis
int Arbol_R_Estrella::escoger_eje_division(Nodo* N){
    // CSA1
    Intervalo k_intervalo{1, Constante::M - 2*Constante::m + 2};
    // [eje x:0, eje y:1][orden_menor:0, orden_mayor:1 ]
    // vector<int> S[2]{{k.mayor-k.menor + 1, 0}, {k.mayor-k.menor + 1, 0}};
    int S[2]{};

    vector<Entrada*> v_entradas = N->entradas;

    Entrada et1, et2; // entrada_temporal
    Nodo nt1, nt2; // nodo_temporal
    et1.puntero_hijo = &nt1;
    et2.puntero_hijo = &nt2;

    sort(v_entradas.begin(), v_entradas.end(), comparar_rectangulo_menor_X);
    S[0] += calcular_margen(k_intervalo, v_entradas, et1, et2);
    sort(v_entradas.begin(), v_entradas.end(), comparar_rectangulo_mayor_X);
    S[0] += calcular_margen(k_intervalo, v_entradas, et1, et2);
    sort(v_entradas.begin(), v_entradas.end(), comparar_rectangulo_menor_Y);
    S[1] += calcular_margen(k_intervalo, v_entradas, et1, et2);
    sort(v_entradas.begin(), v_entradas.end(), comparar_rectangulo_mayor_Y);
    S[1] += calcular_margen(k_intervalo, v_entradas, et1, et2);

    // CSA2
    return S[0]<S[1];
}

// Algorithm ChooseSplitIndex (k, menor/mayor)
pair<int, bool> Arbol_R_Estrella::escoger_indice_division(Nodo* N, int eje){
    Intervalo I{1, Constante::M - 2*Constante::m + 2};
    // indice-k, sobrelapar, area, orden por menor/mayor, 
    tuple<int, int, int, bool> criterio{0,numeric_limits<int>::max(),numeric_limits<int>::max(), false};
    function<bool(Entrada*, Entrada*)> comparadores[2];
    comparadores[0] = (eje? &comparar_rectangulo_menor_Y: &comparar_rectangulo_menor_X);
    comparadores[1] = (eje? &comparar_rectangulo_mayor_Y: &comparar_rectangulo_mayor_X);
    // void* funcion_menor = (eje?); 

    vector<Entrada*> v_entradas = N->entradas;
    for(int b = 0; b<2; b++){
        sort(v_entradas.begin(), v_entradas.end(), comparadores[b]);
        for(int k = I.menor; k<= I.mayor; k++){
            pair<int, int> evaluador = calcular_sobrelapamiento(N, Constante::m - 1 + k);
            if(evaluador.first < get<1>(criterio)){
                get<0>(criterio) = k;
                get<1>(criterio) = evaluador.first;
                get<2>(criterio) = evaluador.second;
                get<3>(criterio) = b;
            }
            else if(evaluador.first == get<1>(criterio) && evaluador.second < get<2>(criterio)){
                get<0>(criterio) = k;
                get<1>(criterio) = evaluador.first;
                get<2>(criterio) = evaluador.second;
                get<3>(criterio) = b;
            }
        }
    }

    return {get<0>(criterio), get<3>(criterio)};
}

// Algorithm OverflowTreatment
Nodo* Arbol_R_Estrella::tratar_desborde(Nodo* N, bool F){
    Nodo* partir = nullptr;
    if(F && N != raiz){
        reinsertar(N);
    }
    else{
        partir = dividir(N);
    }
    return partir;
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