#include"../include/Intervalo.h"

inline bool Intervalo::dentro(int p){
    return menor<=p && p<=mayor;
}