#ifndef INTEFRAZ_H
#define INTEFRAZ_H

#include <SFML/Graphics.hpp>
#include <utility>
#include <iostream>
#include <cstdio>
#include "Arbol_R_Estrella.h"

using namespace std;

class Arbol_R_Estrella;

/**
 * @brief Contiene los objetos SFML y la estructura R-Tree para representar una interfaz gráfica
 * 
 */
class Interfaz
{
public:
    // Constructor por defecto, aplicacion
    Interfaz();
    // Constructor para debuguear
    Interfaz(Arbol_R_Estrella* A);
    /**
     * @brief Ejecuta el renderizado del objeto sf::RenderWindow
     * 
     */
    // Ejecutar la ventana
    void ejecutar();

protected:
    // Definir que evento se solicita (teclas) para definir un metodo del arbol
    void eventos();
    // Metodo que utiliza la inserción de acuerdo al click del puntero 
    void ingresar_coordenada(sf::Event& event);
    // Metodo que utiliza la inserción de acuerdo multiples click del puntero 
    void ingresar_poligono(sf::Event& event);
    // Metodo que utiliza la busqeuda para hallar los vecinos de acuerdo a la posición del puntero
    void buscar_k_coordenadas(sf::Event& event);
    // Metodo que utiliza la eliminación para eliminar la entrada hoja mas cercana  a la posición del puntero
    void eliminar(sf::Event& event);

private:
    // llamada a la función para dibujar el arbol
    void imprimir_arbol_r();
    // Llamada para cada nodo y dibujar el MBR , punto o poligono
    void imprimir_arbol_r_recursivo(Nodo* nodo, int &espacio, int color);
    // DEPRECADA
    string tupla_string(Punto llave_tupla);

    // Arbol
    Arbol_R_Estrella *arbol_r;
    // Ventana
    sf::RenderWindow window;
    // Fuente de puntos (DEPRECADA)
    sf::Font font;
};

#endif