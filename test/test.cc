#include"../include/Arbol_R_Estrella.h"
#include<random>
#include<fstream>
#include<chrono>

using namespace std;
using namespace std::chrono;

int x = 2048, y = 2048;
random_device rd;

vector<Punto> generar_poligono(){
    int l_lado = (x*(rd()%64 + 21))/10000.f;
    int n_lados = rd()%3 + 3;

    Punto trasladar{rd()%2048, rd()%2048};

    vector<Punto> P;
    if(n_lados == 3){
        P = {{0,0}, {l_lado, 0}, {static_cast<int>(round(l_lado/2.f)), static_cast<int>(round(l_lado*sqrt(3)/2.f))}};
    }
    else if(n_lados == 4){
        P = {{0,0},{0,l_lado},{l_lado, l_lado},{l_lado, 0}};
    }
    else{
        P = {{0,0},{0,l_lado},{l_lado, l_lado},{l_lado, 0}};
    }

    for(int i = 0; i<P.size(); i++){
        P[i].x += trasladar.x;
        P[i].y += trasladar.y;
    }

    return P;
}

static void T_I(){
    ofstream of("T_I.csv", ofstream::out);
    for(int i = 0; i<500; i++){
        of<<i+1;
        if(i == 499)
            of<<'\n';
        else
            of<<',';
    }
        Arbol_R_Estrella* a = new Arbol_R_Estrella;
        for(int i = 0; i<1e5; i++){
            // a->insertar({{rand()%2048, rand()%2048}});
            a->insertar(generar_poligono());
            cout<<i<<endl;
        }
        delete a;
    // for(int t = 0; t<100; t++){
    //     Arbol_R_Estrella* a = new Arbol_R_Estrella;
    //     for(int b = 0; b<500; b++){
    //         auto start = high_resolution_clock::now();
    //         for(int i = 0; i<10; i++){
    //             a->insertar(generar_poligono());
    //         }
    //         auto stop = high_resolution_clock::now();
    //         auto duration = duration_cast<nanoseconds>(stop - start);

    //         if(b != 499)
    //             of<<duration.count()<<',';
    //         else
    //             of<<duration.count()<<'\n';
    //     }
    //     cout<<t+1<<endl;
    //     delete a;
    // }
    of.close();
}
void T_B(){

}
void T_D(){

}
void T_K(){

}

int main(){
    T_I();
}