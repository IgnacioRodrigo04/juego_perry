#include "feria.h"
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

#define MOVERSE_ARRIBA 'W'
#define MOVERSE_ABAJO 'S'
#define MOVERSE_DERECHA 'D'
#define MOVERSE_IZQUIERDA 'A'
#define MOVERSE_ARRIBA_MIN 'w'
#define MOVERSE_ABAJO_MIN 's'
#define MOVERSE_DERECHA_MIN 'd'
#define MOVERSE_IZQUIERDA_MIN 'a'
#define CAMUFLARSE 'Q'
#define CAMUFLARSE_MIN 'q'
#define SEGUIR_JUGANDO 0
#define TERMINAR_JUEGO_GANAR 1

//pre: 
//post: chequea si la accion es valida para jugar, devuelve true, false en caso contrario.
bool es_valida(char accion){
    return accion == MOVERSE_ABAJO || accion == MOVERSE_ABAJO_MIN || accion == MOVERSE_ARRIBA || accion == MOVERSE_ARRIBA_MIN || accion == MOVERSE_IZQUIERDA || accion == MOVERSE_IZQUIERDA_MIN || accion == MOVERSE_DERECHA || accion == MOVERSE_DERECHA_MIN || accion == CAMUFLARSE || accion == CAMUFLARSE_MIN;
}

int main(){
    srand ((unsigned)time(NULL));
    juego_t juego;
    inicializar_juego(&juego);
    system("clear");
    int estado_de_juego = estado_juego(juego);
    while (estado_de_juego == SEGUIR_JUGANDO){
        imprimir_terreno(juego);
        char accion;
        scanf("%c", &accion);
        if(es_valida(accion)){
            realizar_jugada(&juego, accion);
            estado_de_juego = estado_juego(juego);
        } else{
            printf("Jugada Invalida\n");
        }
    }
    system("clear");
    if (estado_de_juego == TERMINAR_JUEGO_GANAR){
        printf("GANASTE\n");
    }else{
        printf("PERDISTE\n");
    }
    return 0;
}

