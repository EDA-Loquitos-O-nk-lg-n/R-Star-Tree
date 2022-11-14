#include "../include/Arbol_R_Estrella.h"
#include "../include/Interfaz.h"

int main(int argc, char const *argv[])
{
    Arbol_R_Estrella* arbolito = new Arbol_R_Estrella;

    // arbolito->insertar({{986,683}});
    // arbolito->insertar({{215,877}});
    // arbolito->insertar({{1235,693}});
    // arbolito->insertar({{592,686}});
    // arbolito->insertar({{1121,349}});
    // arbolito->insertar({{527,462}});
    // arbolito->insertar({{959,390}});
    // arbolito->insertar({{1226,663}});
    // arbolito->insertar({{1126,640}});
    // arbolito->insertar({{236,872}});
    // arbolito->insertar({{1068,111}});
    // arbolito->insertar({{529,667}});
    // arbolito->insertar({{1230,282}});
    // arbolito->insertar({{1223,162}});
    // arbolito->insertar({{1235,167}});
    // arbolito->insertar({{1102,429}});
    // arbolito->insertar({{358,922}});
    // arbolito->insertar({{667,369}});
    // arbolito->insertar({{956,293}});
    // arbolito->insertar({{542,311}});
    // arbolito->insertar({{673,1129}});
    // arbolito->insertar({{219,121}});
    // arbolito->insertar({{1037,684}});
    // arbolito->insertar({{1224,498}});
    // arbolito->insertar({{1270,1215}});
    // arbolito->insertar({{1226,513}});
    // arbolito->insertar({{1080,191}});
    // arbolito->insertar({{373,456}});
    // arbolito->insertar({{870,162}});
    // arbolito->insertar({{581,296}});
    // arbolito->insertar({{625,805}});
    // arbolito->insertar({{427,384}});
    // arbolito->insertar({{1005,1236}});
    // arbolito->insertar({{229,146}});
    // arbolito->insertar({{357,1013}});
    // arbolito->insertar({{1195,1024}});
    // arbolito->insertar({{1045,1282}});
    // arbolito->insertar({{667,114}});
    // arbolito->insertar({{864,1134}});
    // arbolito->insertar({{1050,943}});


    Interfaz aplicacion(arbolito);
    aplicacion.ejecutar();

    delete arbolito;    
    return 0;
}
