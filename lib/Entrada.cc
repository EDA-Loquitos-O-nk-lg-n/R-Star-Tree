#include "../include/Entrada.h"

Entrada::Entrada(): puntero_hijo(nullptr){} // situamos a nula el puntero al hijo

Entrada::~Entrada() = default; 

Entrada::Entrada(Nodo* H): puntero_hijo(H){
    // Definimos el puntero al hijo parametro y actualizamos los intervalos del MBR
    actualizar_rectangulo(); 
}

bool Entrada::dentro(int x, int y){
    // verificar si x esta en el intervalo del eje x y si y en el intervalo del eje y
    return rectangulo[0].dentro(x) && rectangulo[1].dentro(y);
}

int Entrada::perimetro(){
    // Calcular la distancia de cada intervalo, sumar, y duplicar
    return (lado_x() + lado_y())<<1;
}

int Entrada::lado_x(){
    // Calcular el lado X del intervalo
    return rectangulo[0].mayor - rectangulo[0].menor;
}
int Entrada::lado_y(){
    // Calcular el lado Y del intervalo
    return rectangulo[1].mayor - rectangulo[1].menor;
}

bool comparar_rectangulo_menor_X(Entrada *e1, Entrada *e2){
    // Comparar con respecto a la cota menor del eje x
    return e1->rectangulo[0].menor < e2->rectangulo[0].menor;
}
bool comparar_rectangulo_menor_Y(Entrada *e1, Entrada *e2){
    // Comparar con respecto a la cota menor del eje y
    return e1->rectangulo[1].menor < e2->rectangulo[1].menor;
}
bool comparar_rectangulo_mayor_X(Entrada *e1, Entrada *e2){
    // Comparar con respecto a la cota mayor del eje x
    return e1->rectangulo[0].mayor < e2->rectangulo[0].mayor;
}
bool comparar_rectangulo_mayor_Y(Entrada *e1, Entrada *e2){
    // Comparar con respecto a la cota mayor del eje y
    return e1->rectangulo[1].mayor < e2->rectangulo[1].mayor;
}

float Entrada::distancia_centros(Entrada* K){
    // Obtener centro de la entrada actual y calcular la distancia al centro de la entrada K
    return centro().distancia(K->centro());
}

inline Punto Entrada::centro(){
    // Promediar las cotas del eje x e y, y generar el par
    return {(rectangulo[0].mayor + rectangulo[0].menor)/2, (rectangulo[1].mayor + rectangulo[1].menor)/2};
}

Entrada::Entrada(const vector<Punto> &P): objeto(P), puntero_hijo(nullptr){
    // Actualizar los valores del intervalo y definir como nulo el puntero al hijo
    actualizar_rectangulo();
}

void Entrada::actualizar_rectangulo(){
    // valores iniciales de comparación
    rectangulo[0] = {numeric_limits<int>::max(), numeric_limits<int>::min()};
    rectangulo[1] = {numeric_limits<int>::max(), numeric_limits<int>::min()};
    // Si es una entrada interna 
    if(puntero_hijo == nullptr){
        // Comparar cada intervalo
        for(Punto p: objeto){
            rectangulo[0].menor = min(rectangulo[0].menor, p.x);
            rectangulo[1].menor = min(rectangulo[1].menor, p.y);
            rectangulo[0].mayor = max(rectangulo[0].mayor, p.x);
            rectangulo[1].mayor = max(rectangulo[1].mayor, p.y);
        }
    }
    else{
        // Comparar cada intervalo
        for(Entrada* e: puntero_hijo->entradas){
            rectangulo[0].menor = min(rectangulo[0].menor, e->rectangulo[0].menor);
            rectangulo[0].mayor = max(rectangulo[0].mayor, e->rectangulo[0].mayor);
            rectangulo[1].menor = min(rectangulo[1].menor, e->rectangulo[1].menor);;
            rectangulo[1].mayor = max(rectangulo[1].mayor, e->rectangulo[1].mayor);
        }
    }
}

int Entrada::area(){
    // Hallar el producto de la distancia de cada eje del itnervalo
    return lado_x()*lado_y();
}

int Entrada::ampliacion(Entrada* K){
    // Obtener el MBR al fusionar el MBR de la entrada actual y la entrada K
    Intervalo nuevo_rectangulo[2];
    nuevo_rectangulo[0] = {min(K->rectangulo[0].menor, rectangulo[0].menor), max(K->rectangulo[0].mayor, rectangulo[0].mayor)};
    nuevo_rectangulo[1] = {min(K->rectangulo[1].menor, rectangulo[1].menor), max(K->rectangulo[1].mayor, rectangulo[1].mayor)};
    
    // Calcular el area actual
    int area_previa = area();
    // Hallar la diferencia de las areas
    int area_ampliada = (nuevo_rectangulo[0].mayor - nuevo_rectangulo[0].menor)*(nuevo_rectangulo[1].mayor - nuevo_rectangulo[1].menor);

    // Retornar el area que se debe ampliar
    return area_ampliada - area_previa;
}

int Entrada::superposicion(Entrada* K){
    // Determinar la distancia del eje x que se sobrelapa entre la entrada actual y la entrada K
    int distancia_x = min(rectangulo[0].mayor, K->rectangulo[0].mayor) - max(rectangulo[0].menor, K->rectangulo[0].menor);
    // Determinar la distancia del eje y que se sobrelapa entre la entrada actual y la entrada K
    int distancia_y = min(rectangulo[1].mayor, K->rectangulo[1].mayor) - max(rectangulo[1].menor, K->rectangulo[1].menor);
    // Hallar la multiplicación de las distancias
    if(distancia_x > 0 && distancia_y > 0)
        return distancia_x*distancia_y;
    return 0;
}
