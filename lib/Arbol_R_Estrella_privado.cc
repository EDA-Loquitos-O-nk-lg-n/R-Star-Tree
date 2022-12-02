#include"../include/Arbol_R_Estrella.h"

void Arbol_R_Estrella::obtener_altura(){
    // atributo de la altura
    altura = 0;
    // Iterador de niveles
    Nodo* N = raiz;
    // Bajar hasta llegar a la hoja
    while(!N->hoja){
        N = N->entradas.front()->puntero_hijo;
        // Aumentar la altura
        altura++;
    }
}

// DEPRECADA
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

    // Area que sobrelapa (INTERSECCIONES)
    int area_sobrelapada = 0;
    // AREA total (MBR que engloba)
    int area_distribucion = 0;

    // MBR 1
    // ALmacenador de indices, set para no repetir
    set<int> seg_set_x, seg_set_y;

    // Para todas las entradas, ingresar las cotas superior e inferior al conjunto de cada eje
    for(int i = 0; i<indice; i++){
        seg_set_x.insert(N->entradas[i]->rectangulo[0].menor);
        seg_set_x.insert(N->entradas[i]->rectangulo[0].mayor);
        seg_set_y.insert(N->entradas[i]->rectangulo[1].menor);
        seg_set_y.insert(N->entradas[i]->rectangulo[1].mayor);
    }

    // Clonar las entradas
    vector<int> segmento_x(seg_set_x.begin(), seg_set_x.end()), segmento_y(seg_set_y.begin(), seg_set_y.end());
    // crear una matriz con las areas por segmentos
    vector<vector<int>> matriz(seg_set_x.size() - 1, vector<int>(seg_set_y.size() - 1, 0));
    
    // Para los indices en eje x
    for(int ix = 1; ix<segmento_x.size(); ix++){
        // Para los indices en eje y
        for(int iy = 1; iy<segmento_y.size(); iy++){
            // Iterar por todas las entradas
            for(int ie = 0; ie<indice; ie++){
                // Si el MBR está adentro del intervalo tanto en eje x como en eje y
                if(N->entradas[ie]->dentro(segmento_x[ix-1], segmento_y[iy-1]) && N->entradas[ie]->dentro(segmento_x[ix], segmento_y[iy])){
                    // Aumentar la casilla de la matriz
                    matriz[ix-1][iy-1]++;
                }
            }
        }
    }

    // Iterar por todas las cuadriculas
    for(int i = 0; i<segmento_x.size()-1; i++){
        for(int j = 0; j<segmento_y.size()-1; j++){
            // Si la casilla tiene mas de 1 (mas de un MBR lo intercepta)
            if(matriz[i][j] > 1){
                // Agregar al area sobrelapada
                area_sobrelapada += (segmento_x[i+1]-segmento_x[i])*(segmento_y[j+1]-segmento_y[j]);
            }
        }
    }

    // Obtener el area del MBR
    area_distribucion += (segmento_x.back() - segmento_x.front())*(segmento_y.back() - segmento_y.front());

    // Limpiar segmentos
    seg_set_x.clear();
    seg_set_y.clear();
    
    // MBR 2
    // Para todas las entradas, ingresar las cotas superior e inferior al conjunto de cada eje
    for(int i = indice; i<N->entradas.size(); i++){
        seg_set_x.insert(N->entradas[i]->rectangulo[0].menor);
        seg_set_x.insert(N->entradas[i]->rectangulo[0].mayor);
        seg_set_y.insert(N->entradas[i]->rectangulo[1].menor);
        seg_set_y.insert(N->entradas[i]->rectangulo[1].mayor);
    }
    // clonar segmentos
    segmento_x = vector<int>(seg_set_x.begin(), seg_set_x.end()); 
    segmento_y = vector<int>(seg_set_y.begin(), seg_set_y.end());
    // Obtener cuadriculas
    matriz = vector<vector<int>>(seg_set_x.size() - 1, vector<int>(seg_set_y.size() - 1, 0));

    // Aumentar si la  cuadriculla esta dentro del MBR
    for(int ix = 1; ix<segmento_x.size(); ix++){
        for(int iy = 1; iy<segmento_y.size(); iy++){
            for(int ie = indice; ie<N->entradas.size(); ie++){
                if(N->entradas[ie]->dentro(segmento_x[ix-1], segmento_y[iy-1]) && N->entradas[ie]->dentro(segmento_x[ix], segmento_y[iy])){
                    matriz[ix-1][iy-1]++;
                }
            }
        }
    }

    // Sumas las cuadriculas que tengan un conteo mayor a 1
    for(int i = 0; i<segmento_x.size()-1; i++){
        for(int j = 0; j<segmento_y.size()-1; j++){
            if(matriz[i][j] > 1){
                area_sobrelapada += (segmento_x[i+1]-segmento_x[i])*(segmento_y[j+1]-segmento_y[j]);
            }
        }
    }
    // Agregamos el area del mBR 2
    area_distribucion += (segmento_x.back() - segmento_x.front())*(segmento_y.back() - segmento_y.front());

    // Devolver el area sobrelapada, y area que engloba  las entradas en MBR 1 y MBR
    return {area_sobrelapada, area_distribucion};
}

int Arbol_R_Estrella::calcular_margen(Intervalo& I, vector<Entrada*>& vE, Entrada& et1, Entrada& et2){
    // Calcular la suma de margenes
    int suma_margen = 0;
    // Para todas las distribuciones (posibles particiones)
    for(int k=I.menor; k<=I.mayor; k++){
        // Asignar las entradas a la particion 1
        et1.puntero_hijo->entradas = vector<Entrada*>(vE.begin(), next(vE.begin(), (Constante::m-1)+k)); 
        // Asignar las entradas a la particion 2
        et2.puntero_hijo->entradas = vector<Entrada*>(next(vE.begin(), (Constante::m-1)+k), vE.end()); 
        // Actualizamos los valores de intervalos
        et1.actualizar_rectangulo();
        // Actualizamos los valores de intervalos
        et2.actualizar_rectangulo();
        // Agregar la suma de los margenes al global
        suma_margen += et1.perimetro() + et2.perimetro();
    }
    // El margen sería la suma de LOS PERIMETROS
    return suma_margen;
}

void Arbol_R_Estrella::destruir_recursivo(Nodo* N){
    // Iterar por todas las entradas del nodo
    for(Entrada* e: N->entradas){
        // Si es un nodo hoja
        if(!N->hoja){
            // Llamamos recursivamente
            destruir_recursivo(e->puntero_hijo);
        }
        // Liberamos la entrada
        delete e;
    }

    // Liberamos el Nodo
    delete N;
}