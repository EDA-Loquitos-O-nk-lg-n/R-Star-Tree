#include"../include/Arbol_R_Estrella.h"

void Arbol_R_Estrella::condensar_cercano(Nodo* L){
    // CT1
    // Iterador de niveles situado en L
    Nodo* N = L;
    // Almacenar de entradas hoja e internas que serían eliminadas por underflow
    vector<Entrada*> Q;
    // CT2
    // Mientras no sea la raiz
    while(N != raiz){
        // Padre del N
        Nodo* P = N->padre;
        vector<Entrada*>::iterator iE_N;
        // Si el nodo fue
        bool eliminado = false;
        // Obtener la entrada que engloba a N
        iE_N = find_if(P->entradas.begin(), P->entradas.end(),[N](Entrada* e){
            return e->puntero_hijo == N;
        });
        // CT3
        // Si hay underflow
        if(N->entradas.size() < Constante::m){
            // Agregar a Q
            Q.push_back(*iE_N);
            // Eliminar la entrada del padre
            P->entradas.erase(iE_N);
            // Nodo eliminado
            eliminado = true;
        }
        // CT4
        // Si el nodo solo tuvo una  reducción de entradas
        if(!eliminado){
            // Actualizar valores de interalos
            (*iE_N)->actualizar_rectangulo();
        }
        // CT5
        // Subir al padre para probar con el siguiente nivel
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
void Arbol_R_Estrella::insercion(Entrada* R, int Nivel, bool Primera_Llamada){
    // I1
    // Empleamos escoger_subarbol para obtener el nodo  apropiado para insertar la nueva entrada
    Nodo* Apropiado{escoger_subarbol(Nivel, R)};
    // I2, I3, I4
    // Puntero que almacenará el nodo partido o solo será NULO
    Nodo* partido;
    // Mientras no se llegue al padre de la raiz (NULO)
    while(Apropiado != nullptr){
        // Si sigue habiendo una entrada (ya sea objeto o nodo) a insertar
        if(R != nullptr){
            // Agregar la entrada a la lista de entradas del nodo apropiado
            Apropiado->entradas.push_back(R);
            // if(!Apropiado->hoja){
            // Si es una entrada interna
            if(R->puntero_hijo != nullptr){
                // Definimos el padre del nodo como el nodo apropiado
                R->puntero_hijo->padre = Apropiado;
            }
        }

        // Si hay overflow
        if(Apropiado->entradas.size() > Constante::M){
            // Llamar al manejo de desborde, lo que retorna la dirección de un nodo, si el actual fue partido
            partido = tratar_desborde(Apropiado, Nivel, Primera_Llamada);
            // terminar la función si esta es la primera vez que se llama a la función y nos encontramos en el nivel de la raiz
            if( Primera_Llamada == true && Apropiado != raiz){
                return;
            }
            // Si la raiz fue partida
            if(partido != nullptr && Apropiado == raiz){
                // Crear una nueva raiz con la antigua raiz y el nuevo nodo como entradas
                raiz = new Nodo{false, nullptr, {new Entrada{raiz}, new Entrada{partido}}};
                // Definir el padre de las entradas como la nueva raiz
                raiz->entradas[0]->puntero_hijo->padre = raiz;
                raiz->entradas[1]->puntero_hijo->padre = raiz;
                // Terminar la función
                return;
            }

            // Si el nodo fue partido
            if(partido != nullptr){
                // Volver el nodo como una entrada para insertarlo en el nivel superior
                R = new Entrada{partido};
            }
            else{
                // Asignar el puntero como NULO ya que no habrá nada que insertar en niveles superiores
                R = nullptr;
            }
        }
        // Si no hay overflow
        else{
            // Asignar el puntero como NULO ya que no habrá nada que insertar en niveles superiores
            R = nullptr;
        }

        // Mientras que no se llegue a la raiz (la raiz no tiene un MBR, solo sus entrada)
        if(Apropiado != raiz){
            // Hallar la entrada cuyo nodo que contiene pueda ser actualizado
            Entrada* E_Apropiado = *find_if(Apropiado->padre->entradas.begin(), Apropiado->padre->entradas.end(), [Apropiado](Entrada* e){return e->puntero_hijo == Apropiado;});
            // Actualizar valores de la entrada
            E_Apropiado->actualizar_rectangulo();
        }
        
        // Subir de nive lal padre del nodo apropiado
        Apropiado = Apropiado->padre;
        // Disminuir el nivel
        Nivel--;
    }
}

// Algorithm ChooseSubtree
Nodo* Arbol_R_Estrella::escoger_subarbol(int Nivel,  Entrada* ER){
    // CS1
    // Empezar con la raiz
    Nodo* N{raiz};

    // CS2
    // Mientras no se llegue a las hojas
    while(Nivel != 0){
        // Si las entradas son hojas
        if(N->entradas.front()->puntero_hijo == nullptr){
            // TUPLA comparadora para obtener el nodo apropiado {puntero_entrada, incremento superposicion, area_ampliada, area}
            tuple<Entrada*, int, int, int> escoger{nullptr, numeric_limits<int>::max(), numeric_limits<int>::max(), numeric_limits<int>::max()};
            // Iterar por las entradas
            for(Entrada* ek: N->entradas){
                // Definir variables temporales para comparar
                int superposicion = 0, nueva_superposicion = 0, ek_area = ek->area(), ek_area_ampliada = ek->ampliacion(ER);
                // Entrada que define el intervalo ampliado en caso la enttrada sea insertada en la entrada ek
                Entrada ek_ampliado;
                // Definir intervalo ampliado en X
                ek_ampliado.rectangulo[0] = {min(ek->rectangulo[0].menor, ER->rectangulo[0].menor), max(ek->rectangulo[0].mayor, ER->rectangulo[0].mayor)};
                // Definir intervalo ampliado en Y
                ek_ampliado.rectangulo[1] = {min(ek->rectangulo[1].menor, ER->rectangulo[1].menor), max(ek->rectangulo[1].mayor, ER->rectangulo[1].mayor)};
                // Determinar la cantidad de area que se sobrelaparía con el resto de nodos 
                for(Entrada* er: N->entradas){
                    if(ek == er)
                        continue;

                    // superposició´n cuando no hay nada agregado
                    superposicion += ek->superposicion(er);
                    // superposició´n cuando se insertase la entrada
                    nueva_superposicion += ek_ampliado.superposicion(er);
                }

                // Si la superposición es menor al valor del comparador
                if(nueva_superposicion - superposicion < get<1>(escoger)){
                    // Redefinir el valor con los datos de la entrada seleccionada
                    escoger = {ek, nueva_superposicion - superposicion, ek_area_ampliada, ek_area};
                }
                // Si tiene el mismo incremento de superposición
                else if(nueva_superposicion - superposicion == get<1>(escoger)){
                    // Desempatar por medio del area que se ampliaría, si es menor
                    if(ek_area_ampliada < get<2>(escoger)){
                    // Redefinir el valor con los datos de  la entrada seleccionada
                        escoger = {ek, nueva_superposicion - superposicion, ek_area_ampliada, ek_area};
                    }
                    // Si ise apmlia la misma cantidad de area
                    else if(ek_area_ampliada == get<2>(escoger)){
                        // Si el area del area del rectangulo es menor al del comparador
                        if(ek_area < get<3>(escoger)){
                        // Redefinir el valor con los datos de  la entrada seleccionada
                            escoger = {ek, nueva_superposicion - superposicion, ek_area_ampliada, ek_area};
                        }
                    }
                }
            }
            // Descender de nivel al hijo de la entrada que fue seleccionada
            N = get<0>(escoger)->puntero_hijo;
        }
        // Si son entradas internas
        else{
            // Hallar la tupla de comparador { puntero-Entrada, area ampliada, area}
            tuple<Entrada*, int, int> escoger{nullptr, numeric_limits<int>::max(), numeric_limits<int>::max()};
            // Iterar por las entradas
            for(Entrada* ei: N->entradas){
                // Definir la popsible cantidad de area ampliada
                int ei_ampliacion{ei->ampliacion(ER)};
                // obtener el area del rectangulo
                int ei_area{ei->area()};

                // Si el area ampliada es menor
                if(ei_ampliacion < get<1>(escoger)){
                    // Redefinir el valor con los datos de  la entrada seleccionada
                    escoger = {ei, ei_ampliacion, ei_area};
                }
                // Si hay empate en ampliación
                else if(ei_ampliacion == get<1>(escoger)){
                    // Si el area del iterado es menor al comparador
                    if(ei_area < get<2>(escoger)){
                        // Redefinir el valor con los datos de  la entrada seleccionada
                        escoger = {ei, ei_ampliacion, ei_area};
                    }
                }
            }
            // Descender de nivel , hallando el nodo seleccionado
            N = get<0>(escoger)->puntero_hijo;
        }
        // Disminuir el nivel
        Nivel--;
    }

    // Retornar el nodo hoja apropiado para lainserción
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
    // Ejecutar la función para obtener que eje es el más efectivo para realizar el ordenamiento
    int eje{escoger_eje_division(N)};

    // S2
    // Obtener el intervalo en el que puede posicionarse el indice para particionar
    pair<int, bool> indice_k_orden{escoger_indice_division(N, eje)};
    // Una vez hallado, ordenamos las entradas dependiendo de si se ordena por medio del lado menor o mayor por un eje definido
    sort(N->entradas.begin(), N->entradas.end(),  comparadores[eje][indice_k_orden.second]);

    // DEBUG
    // cout<<eje<<'\t'<<indice_k_orden.first<<'\t'<<indice_k_orden.second<<'\n';

    // S3
    // Partimos el nodo
    Nodo* N_partido = new Nodo{N->hoja, N->padre}; 
    // Definimos sus entradas desde el indice m - 1 + k hasta el final
    N_partido->entradas = vector<Entrada*>(next(N->entradas.begin(), Constante::m - 1 + indice_k_orden.first), N->entradas.end());
    // Recortamos las entradas del nodo N para que tenga las entradas 1 hasta m-1+k
    N->entradas.resize(Constante::m - 1 + indice_k_orden.first);
    //Si el nodo partido es un nodo interno
    if(!N_partido->hoja){
        // Para cada entrada del nodo interno que tienen un nodo hijo
        for(Entrada* e: N_partido->entradas){
            // Definimos el padre de cada nodo en la entrada como el nodo partido
            e->puntero_hijo->padre = N_partido;
        }
    }
    return N_partido;
}

// Algorithm ChooseSpliAxis
int Arbol_R_Estrella::escoger_eje_division(Nodo* N){
    // CSA1
    // Definir el intervalo del indice de particion
    Intervalo k_intervalo{1, Constante::M - 2*Constante::m + 2};
    // [eje x:0, eje y:1][orden_menor:0, orden_mayor:1 ]
    // vector<int> S[2]{{k.mayor-k.menor + 1, 0}, {k.mayor-k.menor + 1, 0}};
    // Variables que almacenan las areas de todas las distribuciones, por cada eje
    int S[2]{};

    // Otener temporalmente las entradas
    vector<Entrada*> v_entradas = N->entradas;

    Entrada et1, et2; // entrada_temporal
    Nodo nt1, nt2; // nodo_temporal

    // Definimos los punteros hijos
    et1.puntero_hijo = &nt1;
    et2.puntero_hijo = &nt2;

    // Ordenar en eje x, desde la cota menor del intervalo
    sort(v_entradas.begin(), v_entradas.end(), comparar_rectangulo_menor_X);
    // Obtener el margen de todas las distribuciones
    S[0] += calcular_margen(k_intervalo, v_entradas, et1, et2);
    // Ordenar en eje x, desde la cota mayor del intervalo
    sort(v_entradas.begin(), v_entradas.end(), comparar_rectangulo_mayor_X);
    // Obtener el margen de todas las distribuciones
    S[0] += calcular_margen(k_intervalo, v_entradas, et1, et2);
    // Ordenar en eje y, desde la cota menor del intervalo
    sort(v_entradas.begin(), v_entradas.end(), comparar_rectangulo_menor_Y);
    // Obtener el margen de todas las distribuciones
    S[1] += calcular_margen(k_intervalo, v_entradas, et1, et2);
    // Obtener el margen de todas las distribuciones
    sort(v_entradas.begin(), v_entradas.end(), comparar_rectangulo_mayor_Y);
    // Ordenar en eje y, desde la cota mayor del intervalo
    S[1] += calcular_margen(k_intervalo, v_entradas, et1, et2);

    // CSA2
    // Determinar si la suma en y es mayor que en x. 0: ejex, 1: ejey
    return S[0]<S[1];
}

// Algorithm ChooseSplitIndex (k, menor/mayor)
pair<int, bool> Arbol_R_Estrella::escoger_indice_division(Nodo* N, int eje){
    // Intervalo para el indice de division
    Intervalo I{1, Constante::M - 2*Constante::m + 2};
    // TUPLA comoparadora < indice-k, sobrelapar, area, orden por menor/mayor  > 
    tuple<int, int, int, bool> criterio{0,numeric_limits<int>::max(),numeric_limits<int>::max(), false};
    // Determinar si las funciones para obtener  son en eje y o eje x
    function<bool(Entrada*, Entrada*)> comparadores[2];
    comparadores[0] = (eje? &comparar_rectangulo_menor_Y: &comparar_rectangulo_menor_X);
    comparadores[1] = (eje? &comparar_rectangulo_mayor_Y: &comparar_rectangulo_mayor_X);

    // Almacenar temporalmente las entradas dle nodo
    vector<Entrada*> v_entradas = N->entradas;
    // Para ordenar por cota inferior y superior
    for(int b = 0; b<2; b++){
        // Ordenar por la función
        sort(v_entradas.begin(), v_entradas.end(), comparadores[b]);
        // Definir diferentes indices de particion
        for(int k = I.menor; k<= I.mayor; k++){
            // Obtener la cantidad de solapamiento si se parte en aquel indice
            pair<int, int> evaluador = calcular_sobrelapamiento(N, Constante::m - 1 + k);
            // Si el area sobrelapada es menor que el comparador
            if(evaluador.first < get<1>(criterio)){
                // Redefinir valores
                get<0>(criterio) = k;
                get<1>(criterio) = evaluador.first;
                get<2>(criterio) = evaluador.second;
                get<3>(criterio) = b;
            }
            // Si el area sobrelapada es igual que el comparador pero el area que engloba es meno al del comparador
            else if(evaluador.first == get<1>(criterio) && evaluador.second < get<2>(criterio)){
                // Redefinir valores
                get<0>(criterio) = k;
                get<1>(criterio) = evaluador.first;
                get<2>(criterio) = evaluador.second;
                get<3>(criterio) = b;
            }
        }
    }

    // Retornar {indice de particion k, si el orden se hace mdiante cota inferior o superior}
    return {get<0>(criterio), get<3>(criterio)};
}

// Algorithm OverflowTreatment
Nodo* Arbol_R_Estrella::tratar_desborde(Nodo* N, int Nivel, bool Primera_Llamada){
    // Nodo que almacene el puntero al nodo en caso sea particionado
    Nodo* partir = nullptr;
    // Si es la primera vez que se llama al manejo de overflow
    if(Primera_Llamada && N != raiz){
        // Procedemos con la reinserción forzada
        reinsertar(N, Nivel);
    }
    else{
        // Particionar nodo
        partir = dividir(N);
    }
    // Retornar el nuevo nodo o un NULO
    return partir;
}

// Algorithm ReInsert
void Arbol_R_Estrella::reinsertar(Nodo* N, int Nivel){
    // Obtener la entrada cuyo hijo es N
    Entrada* e = nullptr;
    for(Entrada* et: N->padre->entradas){
        if(et->puntero_hijo == N){
            e = et;
            break;
        }
    }

    // RI1
    // RI2
    // Cola de prioridad para almacenar la distancia de una entrada al centro del mbr que engloba al nodo
    priority_queue<pair<float, Entrada*>, deque<pair<float, Entrada*>>, greater<pair<float, Entrada*>>> distancias;
    // Por cada entrada de N
    for(Entrada* et: N->entradas){
        // Obtener la distancia entre punto medio de et a la entrada de N
        distancias.push({e->distancia_centros(et), et});
    }
    // Limpiar las entradas de N
    N->entradas.clear();

    // RI3
    // Se trata de conservar el 70% de entradas del nodo
    float conservar = ceil(distancias.size() * 0.7f);
    for(int i = 0; i<conservar; i++){
        // Se ingresan de manera ordenada aquellas entradas con menor distancia al centro
        N->entradas.push_back(distancias.top().second);
        // desencolar
        distancias.pop();
    }
    // Reajustar rectangulo desde el nodo hasta la entrada de la raiz
    for(Nodo* nt = N; nt != raiz; nt = nt->padre){
        for(Entrada* et: nt->padre->entradas){
            if(et->puntero_hijo == nt){
                et->actualizar_rectangulo();
                break;
            }
        }
    }

    // RI4
    // Obtenemos la altura actual
    int altura_actual = altura;
    // Mientras la cola no esté vacía
    while(!distancias.empty()){
        // Reinsertar el 30% sobrante de entradas en la cola
        insercion(distancias.top().second, Nivel + (altura - altura_actual), false);
        // Recalculamos la alutra
        obtener_altura();
        // Desencolamos
        distancias.pop();
    }
}