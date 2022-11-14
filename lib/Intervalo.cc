#include"../include/Intervalo.h"

bool Intervalo::dentro(int p){
    return menor<=p && p<=mayor;
}