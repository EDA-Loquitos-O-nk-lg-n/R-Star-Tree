#include"../include/Intervalo.h"

// Determinar si un valor esta dentro del intervalo
bool Intervalo::dentro(int p){
    // Debe ser mayor-igual que la cota inferior y menor-igual que la cota mayor
    return menor<=p && p<=mayor;
}