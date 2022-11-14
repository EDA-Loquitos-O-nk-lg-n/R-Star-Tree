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
    Interfaz();
    Interfaz(Arbol_R_Estrella* A);
    /**
     * @brief Ejecuta el renderizado del objeto sf::RenderWindow
     * 
     */
    void ejecutar();

protected:
    void eventos();
    void ingresar_coordenada(sf::Event& event);
    void ingresar_poligono(sf::Event& event);
    void buscar_k_coordenadas(sf::Event& event);
    void eliminar(sf::Event& event);

private:
    void imprimir_arbol_r();
    void imprimir_arbol_r_recursivo(Nodo* nodo, int &espacio, int color);
    string tupla_string(Punto llave_tupla);

    Arbol_R_Estrella *arbol_r;
    sf::RenderWindow window;
    sf::Font font;
};

#endif