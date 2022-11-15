#include "../include/Arbol_R_Estrella.h"
#include "../include/Interfaz.h"
#include "debug.h"

int main(int argc, char const *argv[])
{
    Arbol_R_Estrella* arbolito = new Arbol_R_Estrella;

    // debug(arbolito);

    Interfaz aplicacion(arbolito);
    aplicacion.ejecutar();

    delete arbolito;    
    return 0;
}
