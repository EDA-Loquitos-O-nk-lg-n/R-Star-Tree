#include "../include/Entrada.h"

Entrada::Entrada(): puntero_hijo(nullptr){}

Entrada::~Entrada() = default;

Entrada::Entrada(Nodo* H): puntero_hijo(H){
    actualizar_rectangulo();
}

int Entrada::perimetro(){
    return (lado_x() + lado_y())<<1;
}

int Entrada::lado_x(){
    return rectangulo[0].mayor - rectangulo[0].menor;
}
int Entrada::lado_y(){
    return rectangulo[1].mayor - rectangulo[1].menor;
}

bool comparar_rectangulo_menor_X(Entrada *e1, Entrada *e2){
    return e1->rectangulo[0].menor < e2->rectangulo[0].menor;
}
bool comparar_rectangulo_menor_Y(Entrada *e1, Entrada *e2){
    return e1->rectangulo[1].menor < e2->rectangulo[1].menor;
}
bool comparar_rectangulo_mayor_X(Entrada *e1, Entrada *e2){
    return e1->rectangulo[0].mayor < e2->rectangulo[0].mayor;
}
bool comparar_rectangulo_mayor_Y(Entrada *e1, Entrada *e2){
    return e1->rectangulo[1].mayor < e2->rectangulo[1].mayor;
}

float Entrada::distancia_centros(Entrada* K){
    return centro().distancia(K->centro());
}

inline Punto Entrada::centro(){
    return {(rectangulo[0].mayor + rectangulo[0].menor)/2, (rectangulo[1].mayor + rectangulo[1].menor)/2};
}

Entrada::Entrada(const vector<Punto> &P): puntero_hijo(nullptr){
    actualizar_rectangulo();
}

void Entrada::actualizar_rectangulo(){
    rectangulo[0] = {numeric_limits<int>::max(), numeric_limits<int>::min()};
    rectangulo[1] = {numeric_limits<int>::max(), numeric_limits<int>::min()};
    if(puntero_hijo == nullptr){
        for(Punto p: objeto){
            rectangulo[0].menor = min(rectangulo[0].menor, p.x);
            rectangulo[1].menor = min(rectangulo[1].menor, p.y);
            rectangulo[0].mayor = max(rectangulo[0].mayor, p.x);
            rectangulo[1].mayor = max(rectangulo[1].mayor, p.y);
        }
    }
    else{
        for(Entrada* e: puntero_hijo->entradas){
            rectangulo[0].menor = min(rectangulo[0].menor, e->rectangulo[0].menor);
            rectangulo[0].mayor = max(rectangulo[0].mayor, e->rectangulo[0].mayor);
            rectangulo[1].menor = min(rectangulo[1].menor, e->rectangulo[1].menor);;
            rectangulo[1].mayor = max(rectangulo[1].mayor, e->rectangulo[1].mayor);
        }
    }
}

int Entrada::area(){
    return lado_x()*lado_y();
}

int Entrada::ampliacion(Entrada* K){
    Intervalo nuevo_rectangulo[2];
    nuevo_rectangulo[0] = {min(K->rectangulo[0].menor, rectangulo[0].menor), max(K->rectangulo[0].mayor, rectangulo[0].mayor)};
    nuevo_rectangulo[1] = {min(K->rectangulo[1].menor, rectangulo[1].menor), max(K->rectangulo[1].mayor, rectangulo[1].mayor)};
    
    int area_previa = area();
    int area_ampliada = (nuevo_rectangulo[0].mayor - nuevo_rectangulo[0].menor)*(nuevo_rectangulo[1].mayor - nuevo_rectangulo[1].menor);

    return area_ampliada - area_previa;
}

int Entrada::superposicion(Entrada* K){
    int distancia_x = min(rectangulo[0].mayor, K->rectangulo[0].mayor) - max(rectangulo[0].menor, K->rectangulo[0].menor);
    int distancia_y = min(rectangulo[1].mayor, K->rectangulo[1].mayor) - max(rectangulo[1].menor, K->rectangulo[1].menor);
    if(distancia_x > 0 && distancia_y > 0)
        return distancia_x*distancia_y;
    return 0;
}
