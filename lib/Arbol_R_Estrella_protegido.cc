#include"../include/Arbol_R_Estrella.h"

void Arbol_R_Estrella::condensar_cercano(Nodo* L){
    // CT1
    Nodo* N = L;
    vector<Entrada*> Q;
    // CT2
    while(N != raiz){
        Nodo* P = N->padre;
        vector<Entrada*>::iterator iE_N;
        bool eliminado = false;
        iE_N = find_if(P->entradas.begin(), P->entradas.end(),[N](Entrada* e){
            return e->puntero_hijo == N;
        });
        // CT3
        if(N->entradas.size() < Constante::m){
            Q.push_back(*iE_N);
            P->entradas.erase(iE_N);
            eliminado = true;
        }
        // CT4
        if(!eliminado){
            (*iE_N)->actualizar_rectangulo();
        }
        // CT5
        N = P;
    }
    // CT6
    for(int i = 0; i<Q.size(); i++){
        for(Entrada *e: Q[i]->puntero_hijo->entradas){
            insercion(e, altura-i, true); 
        }
        delete Q[i]->puntero_hijo, Q[i];
    }
}

// Algorithm Insert
void Arbol_R_Estrella::insercion(Entrada* R, int Nivel, bool F){
    // I1
    Nodo* Apropiado{escoger_subarbol(Nivel, R)};
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
            partido = tratar_desborde(Apropiado, Nivel, F);
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
        Nivel--;
    }
}

// Algorithm ChooseSubtree
Nodo* Arbol_R_Estrella::escoger_subarbol(int Nivel,  Entrada* ER){
    // CS1
    Nodo* N{raiz};

    // CS2
    while(Nivel != 0){
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
        Nivel--;
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
Nodo* Arbol_R_Estrella::tratar_desborde(Nodo* N, int Nivel, bool F){
    Nodo* partir = nullptr;
    if(F && N != raiz){
        reinsertar(N, Nivel);
    }
    else{
        partir = dividir(N);
    }
    return partir;
}

// Algorithm ReInsert
void Arbol_R_Estrella::reinsertar(Nodo* N, int Nivel){
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
        insercion(distancias.top().second, Nivel, false);
        distancias.pop();
    }
}