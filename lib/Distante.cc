#include"../include/Arbol_R_Estrella.h"

Arbol_R_Estrella::Distante::Distante(Entrada* E, Punto P, Nodo *N): entrada(E), nodo(N){
    // Si el nodo no es hoja
    if(!N->hoja){
        // En caso el punto P se halle en el MBR, la distancia es 0
        if(P.x >= E->rectangulo[0].menor && P.x <= E->rectangulo[0].mayor && P.y >= E->rectangulo[1].menor && P.y <= E->rectangulo[1].mayor)
            distancia = 0;
        // Si el punto esta afuera del MBR en el eje y, calcular la distancia
        else if(P.x >= E->rectangulo[0].menor && P.x <= E->rectangulo[0].mayor)
            distancia = min(abs(P.y-E->rectangulo[1].menor), abs(P.y-E->rectangulo[1].mayor));
        // Si el punto esta afuera del MBR en el eje x, calcular la distancia
        else if(P.y >= E->rectangulo[1].menor && P.y <= E->rectangulo[1].mayor)
            distancia = min(abs(P.x-E->rectangulo[0].menor), abs(P.x-E->rectangulo[0].mayor));
        // Si el punto no se engloba en el intervalo tanto en el eje x como y, hallar la distancia euclideana
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
        // Si es solo un punto, hallar la distancia entre ambos puntos
        if(E->objeto.size() == 1){
            distancia = sqrt(pow(P.x-E->objeto[0].x,2)+pow(P.y-E->objeto[0].y,2));
        }
        // Si es un poligono, hallar el centro y calcular la distancia
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

Arbol_R_Estrella::Distante::~Distante() = default;

bool operator<(const Arbol_R_Estrella::Distante &pd1, const Arbol_R_Estrella::Distante &pd2){
    // Comparar <= para priority_queue
    return pd1.distancia < pd2.distancia;
}

bool operator>(const Arbol_R_Estrella::Distante &pd1, const Arbol_R_Estrella::Distante &pd2){
    // Comparar > para priority_queue
    return pd1.distancia>pd2.distancia;
}

bool operator<=(const Arbol_R_Estrella::Distante &pd1, const Arbol_R_Estrella::Distante &pd2){
    // Comparar <= para priority_queue
    return pd1.distancia<=pd2.distancia;
}

bool operator>=(const Arbol_R_Estrella::Distante &pd1, const Arbol_R_Estrella::Distante &pd2){
    // Comparar >= para priority_queue
    return pd1.distancia>=pd2.distancia;
}