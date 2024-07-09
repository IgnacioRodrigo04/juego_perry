#include "feria.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <time.h>

const char MOVERSE_ARRIBA = 'W';
const char MOVERSE_ABAJO = 'S';
const char MOVERSE_DERECHA = 'D';
const char MOVERSE_IZQUIERDA = 'A';
const char MOVERSE_ARRIBA_MIN = 'w';
const char MOVERSE_ABAJO_MIN = 's';
const char MOVERSE_DERECHA_MIN = 'd';
const char MOVERSE_IZQUIERDA_MIN = 'a';
const char CAMUFLARSE = 'Q';
const char CAMUFLARSE_MIN = 'q';
const char PERRY = 'P';
const char PHINEAS = 'H';
const char FERB = 'F';
const char CANDACE = 'C';
const char SOMBRERO = 'S';
const char GOLOSINA = 'G';
const char ROBOTS = 'R';
const char BOMBA = 'B';
const char VACIO = '_';
const int VIDAS_INICIALES = 3;
const int ENERGIA_PERRY = 100;
const int TIMER_MIN = 50;
const int TIMER_MAX = 251;
const int MAX_FIL = 20;
const int MAX_COL = 20;
const int CANT_BOMBAS = 10;
const int CANT_SOMBREROS = 3;
const int CANT_GOLOSINAS = 5;
const int MAX_TABLERO = 19;
const int MIN_TABLERO = 0;
const int MAX_SENTIDOS = 4;
const int CANT_FAMILIARES = 3;
const int MAX_COORDENADAS = 400;
const int NO_MAS_VIDAS = 0;
const int ENERGIA_DESACTIVAR_BOMBA = 10;
const int ENERGIA_DESTRUIR_ROBOT = 5;
const int RECARGA_ENERGIA_GOLOSINA = 20;
const int DISTANCIA_FAMILIARES = 1;
const int DISTANCIA_ROBOTS = 2;
const int NO_MAS_TIEMPO = 0;
const int BOMBA_NO_ENCONTRADA = -1;
const int HERRAMIENTA_NO_ENCONTRADA = -1;
const int ROBOT_NO_ENCONTRADO = -1;
const int NINGUN_FAMILIAR = 0;
const int NO_MAS_ROBOTS = 0;
const int CANT_CREAR_ROBOTS = 10;
const int TOPE_PERRY = 1;
const int SEGUIR_JUGANDO = 0;
const int TERMINAR_JUEGO_GANAR = 1;
const int TERMINAR_JUEGO_PERDER = -1;



//pre: las coordenadas tienen que ser validas.
//post: agrega las coordenadas a la lista de coordenadas y suma el tope.
void agregar_coordenadas_a_lista(coordenada_t coordenadas[MAX_COORDENADAS], int* tope_coordenadas, coordenada_t coordenada){
    if(*tope_coordenadas < MAX_COORDENADAS){
        coordenadas[*tope_coordenadas] = coordenada;
        (*tope_coordenadas)++;
    }
}

//pre: tope coordenadas siempre es mayor a 0 y menor a max_coordenadas.
//post: chequea que las coordenadas no esten en el vector de coordenadas, devuelve true si estan false en caso contrario.
bool existe_coordenadas(coordenada_t coordenadas[MAX_COORDENADAS] , int tope_coordenadas, coordenada_t coordenada){
    int i = 0;
    bool existe_coordenada = false;
    while(i < tope_coordenadas && !existe_coordenada){
        if(coordenadas[i].fil == coordenada.fil && coordenadas[i].col == coordenada.col){
            existe_coordenada = true;
        }
        i++;
    }
    return existe_coordenada;
}

//pre: 
//post: inicializa las coordenadas, chequea que no haya otro objeto y las agrega al vector de coordenadas. 
coordenada_t obtener_coordenada_libre(coordenada_t coordenadas[MAX_COORDENADAS], int* tope_coordenadas){
    coordenada_t coordenada;
    coordenada.fil = rand()% MAX_FIL;
    coordenada.col = rand()% MAX_COL;
    while (existe_coordenadas(coordenadas, *tope_coordenadas, coordenada)){
        coordenada.fil = rand()% MAX_FIL;
        coordenada.col = rand()% MAX_COL;
    }
    agregar_coordenadas_a_lista(coordenadas, tope_coordenadas, coordenada);    
    return coordenada;
}

//pre: inicializar coordenada.
//post: inicializa el personaje y sus atributos.
personaje_t inicializar_personaje(coordenada_t coordenadas[MAX_COORDENADAS], int* tope_coordenadas){
    personaje_t personaje;    
    personaje.vida = VIDAS_INICIALES;
    personaje.energia = ENERGIA_PERRY;
    personaje.camuflado = false;
    personaje.posicion = obtener_coordenada_libre(coordenadas, tope_coordenadas);
    return personaje;

}

//pre: inicializar coordenada. 
//post: inicializa la bomba y sus atributos.
bomba_t inicializar_bomba(coordenada_t coordenadas[MAX_COORDENADAS], int* tope_coordenadas){
    bomba_t bomba;
    bomba.posicion = obtener_coordenada_libre(coordenadas, tope_coordenadas);
    bomba.timer = rand()% TIMER_MAX + TIMER_MIN;
    bomba.desactivada = false;
    return bomba;
}

//pre: tope_bombas y tope_coordenadas son válidos y hay suficiente espacio en el arreglo bombas para CANT_BOMBAS bombas.
//post: carga el vector de bombas donde cada posicion es una bomba_t bomba.
void crear_bombas(bomba_t bombas[MAX_BOMBAS], int* tope_bombas, coordenada_t coordenadas[MAX_COORDENADAS], int* tope_coordenadas){
    for(int i = 0;i < CANT_BOMBAS; i++){
        bombas[i] = inicializar_bomba(coordenadas, tope_coordenadas);
        (*tope_bombas)++;
    }
}


//pre: inicializar coordenadas.
//post: inicializa la herramientas y sus atributos.
herramienta_t inicializar_herramienta(char inicial, coordenada_t coordenadas[MAX_COORDENADAS], int* tope_coordenadas){
    herramienta_t herramienta;
    herramienta.posicion = obtener_coordenada_libre(coordenadas, tope_coordenadas);
    herramienta.tipo = inicial;
    return herramienta;
}

//pre: tope_herramientas y tope_coordenadas son válidos y hay suficiente espacio en el arreglo herramientas para CANT_SOMBREROS + CANT_GOLOSINAS herramientas.
//post: carga el vector de herramientas donde cada posicion es una herramienta_t herramienta.
void crear_herramientas(herramienta_t herramientas[MAX_HERRAMIENTAS], int* tope_herramientas, coordenada_t coordenadas[MAX_COORDENADAS], int* tope_coordenadas){
    char inicial = SOMBRERO;
    for(int i = 0; i < CANT_SOMBREROS; i++){
        herramientas[i] = inicializar_herramienta(inicial, coordenadas, tope_coordenadas);
        (*tope_herramientas)++;
    }
    inicial = GOLOSINA;
    for(int i = 0; i < CANT_GOLOSINAS; i++){
        herramientas[*tope_herramientas] = inicializar_herramienta(inicial, coordenadas, tope_coordenadas);
        (*tope_herramientas)++;
    }
}

//pre: inicializar coordenadas.
//post: inicializa el familiar y sus atributos.
familiar_t inicializar_familiar(char inicial, char sentido, coordenada_t coordenadas[MAX_COORDENADAS], int* tope_coordenadas){
    familiar_t familiar;
    familiar.posicion = obtener_coordenada_libre(coordenadas, tope_coordenadas);
    familiar.inicial_nombre = inicial;
    familiar.sentido = sentido;
    return familiar;
}

//pre: MAX_FAMILIARES y MAX_SENTIDOS >= 3
//post: ordena las iniciales de los familiares.
void ordenar_iniciales_y_sentidos_familiares(char iniciales[MAX_FAMILIARES], char sentidos_iniciales[MAX_SENTIDOS]){
    iniciales[0] = PHINEAS;
    sentidos_iniciales[0] = MOVERSE_DERECHA;
    iniciales[1] = FERB;
    sentidos_iniciales[1] = MOVERSE_IZQUIERDA;
    iniciales[2] = CANDACE;
    sentidos_iniciales[2] = MOVERSE_ARRIBA;
}

//pre: tope_familiares y tope_coordenadas son válidos y hay suficiente espacio en el arreglo familiares para CANT_FAMILIARES familiares
//post: carga el vector de familiares donde cada posicion es un familiar_t familiar.
void crear_familiar(familiar_t familiares[MAX_FAMILIARES], int* tope_familiares, char iniciales[MAX_FAMILIARES], char sentidos_iniciales[MAX_SENTIDOS], coordenada_t coordenadas[MAX_COORDENADAS], int* tope_coordenadas) {
    ordenar_iniciales_y_sentidos_familiares(iniciales, sentidos_iniciales);
    for(int i = 0; i < CANT_FAMILIARES; i++){
        familiares[i] = inicializar_familiar(iniciales[i], sentidos_iniciales[i], coordenadas, tope_coordenadas);
        (*tope_familiares)++;
    }
}

void inicializar_juego(juego_t* juego){
    coordenada_t coordenadas[MAX_COORDENADAS];
    int tope_coordenadas = 0;
    juego -> perry = inicializar_personaje(coordenadas, &tope_coordenadas);           
    juego -> tope_bombas = 0;
    crear_bombas(juego -> bombas, &(juego->tope_bombas), coordenadas, &tope_coordenadas);
    juego -> tope_herramientas = 0;
    crear_herramientas(juego -> herramientas,&(juego -> tope_herramientas), coordenadas, &tope_coordenadas);
    juego -> tope_familiares = 0;
    char iniciales_familiares[MAX_FAMILIARES];
    char sentidos_iniciales_familares[MAX_SENTIDOS];
    crear_familiar(juego -> familiares, &(juego->tope_familiares), iniciales_familiares, sentidos_iniciales_familares, coordenadas, &tope_coordenadas);
    juego -> movimientos = 0;
    juego -> cantidad_robots = 0;
}

//pre: inicializar coordenada.
//post: chequea antes de moverse hacia arriba si se va del tablero, si no pasa, se mueve.
void mover_arriba(int *coordenada_i){
    if(*coordenada_i > MIN_TABLERO){
        (*coordenada_i)--;
        
    }
}

//pre: inicializar coordenada.
//post: chequea antes de moverse hacia abajo si se va del tablero, si no pasa, se mueve.
void mover_abajo(int *coordenada_i){
    if(*coordenada_i < MAX_TABLERO){
        (*coordenada_i)++;
        
    }
}

//pre: inicializar coordenada.
//post: chequea antes de moverse hacia izquierda si se va del tablero, si no pasa, se mueve.
void mover_izquierda(int *coordenada_j){
    if(*coordenada_j > MIN_TABLERO){
        (*coordenada_j)--;
        
    }
}

//pre:inicializar coordenada.
//post: chequea antes de moverse hacia derecha si se va del tablero, si no pasa, se mueve.
void mover_derecha(int *coordenada_j){
    if(*coordenada_j < MAX_TABLERO){
        (*coordenada_j)++;
        
    }
}

//pre: tope bombas es valido.
//post: baja el timer un segundo de las bombas que no esten desactivadas.
void bajar_timer_bombas(bomba_t bombas[MAX_BOMBAS], int tope_bombas){
    for(int i = 0; i < tope_bombas; i++){
        if(bombas[i].timer > NO_MAS_TIEMPO && !(bombas[i].desactivada)){
            (bombas[i].timer)--;
        }
    }   
}

//pre: la accion pasada por parametro tiene que ser valida.
//post: realiza la accion pasada por parametro.
void realizar_accion(personaje_t *personaje, char accion, int *movimientos){
    if(accion == MOVERSE_ARRIBA || accion == MOVERSE_ARRIBA_MIN){
        if(personaje->posicion.fil > MIN_TABLERO){
            mover_arriba(&(personaje->posicion.fil));
            (*movimientos)++;
        }
    } else if(accion == MOVERSE_ABAJO || accion == MOVERSE_ABAJO_MIN){
        if(personaje->posicion.fil < MAX_TABLERO){
            mover_abajo(&(personaje->posicion.fil));
            (*movimientos)++;
        }
        
    } else if(accion == MOVERSE_DERECHA || accion == MOVERSE_DERECHA_MIN){
        if(personaje->posicion.col < MAX_TABLERO){
            mover_derecha(&(personaje->posicion.col));  
            (*movimientos)++; 
        }
    } else if(accion == MOVERSE_IZQUIERDA || accion == MOVERSE_IZQUIERDA_MIN){
        if(personaje->posicion.col > MIN_TABLERO){
            mover_izquierda(&(personaje->posicion.col)); 
            (*movimientos)++;
        }
    } else{
        personaje->camuflado = !(personaje->camuflado);
        (*movimientos)++;
    }
}

//pre: familiar.sentido solo puede ser "W" "A" "S" "D".
//post: mueve el familiar de acuerdo a su posicion, si llega al borde del tablero cambia su sentido al opuesto y se mueve en sentido contrario.
void mover_familiar(familiar_t *familiar){
    if(familiar->sentido == MOVERSE_DERECHA){
        if(familiar->posicion.col != MAX_TABLERO){
            mover_derecha(&(familiar->posicion.col));
        } else{
            familiar->sentido = MOVERSE_IZQUIERDA;
            mover_izquierda(&(familiar->posicion.col));
        }
    } else if(familiar->sentido == MOVERSE_IZQUIERDA){
        if(familiar->posicion.col != MIN_TABLERO){
            mover_izquierda(&(familiar->posicion.col));
        } else {
            familiar->sentido = MOVERSE_DERECHA;
            mover_derecha(&(familiar->posicion.col));
        }
    } else if(familiar->sentido == MOVERSE_ARRIBA){
        if(familiar->posicion.fil != MIN_TABLERO){
            mover_arriba(&(familiar->posicion.fil));
        } else {
            familiar->sentido = MOVERSE_ABAJO;
            mover_abajo(&(familiar->posicion.fil));
        }
    } else{
        if(familiar->posicion.fil != MAX_TABLERO){
            mover_abajo(&(familiar->posicion.fil));
        } else{
            familiar->sentido = MOVERSE_ARRIBA;
            mover_arriba(&(familiar->posicion.fil));
        }
    }
}

//pre: tope herramientas >= 1.
//post: elimina un elemento del vector, y lo deja desordenado.
void eliminar_herramienta(herramienta_t herramientas[MAX_HERRAMIENTAS], int *tope_herramientas, int herramienta_a_eliminar){
    herramienta_t aux = herramientas[*(tope_herramientas) - 1];
    herramientas[*(tope_herramientas) - 1] = herramientas[herramienta_a_eliminar];
    herramientas[herramienta_a_eliminar] = aux;
    (*tope_herramientas)--;
        
}

//pre:  personaje y herramienta son válidos.
//post: si perry agarra el sombrero, recibe 1 vida, si perry agarra la golosina suma 20 de energia.
void agarrar_herramienta(personaje_t *personaje, herramienta_t herramienta,  int tope_herramientas){
    if(herramienta.tipo == SOMBRERO){
        personaje->vida++;
    } else if(herramienta.tipo == GOLOSINA){
    personaje->energia += RECARGA_ENERGIA_GOLOSINA;
    }
    
}

//pre: personaje y bomba son válidos, personaje tiene suficiente energía para desactivar la bomba
//post: la bomba actual pasa a estar desactivada, perry pierde 10 de energia.
void desactivar_bomba(personaje_t *personaje, bomba_t *bomba){
    bomba->desactivada = true;
    personaje->energia -= ENERGIA_DESACTIVAR_BOMBA;
}

//pre: el vector de bombas no puede estar vacio.
//post: devuelve la posicion de la bomba que hay que desactivar o -1 si no hay que desactivar nada.
int bomba_a_desactivar(bomba_t bombas[MAX_BOMBAS], int tope_bombas, coordenada_t personaje_posicion){
    bool encontrado = false;
    int pos_bomba_superesta = BOMBA_NO_ENCONTRADA;
    int i = 0;
    while (i < tope_bombas && !encontrado){
        if(bombas[i].posicion.col == personaje_posicion.col && bombas[i].posicion.fil == personaje_posicion.fil){
            pos_bomba_superesta = i;
            encontrado = true;
        }
        i++;
    }
    return pos_bomba_superesta;
}



//pre: vector de herramientas no puede estar vacio.
//post: devuelve la posicion de la herramienta que hay que agarrar o -1 si no hay ninguna en esa posicion.
int herramienta_a_agarrar(herramienta_t herramientas[MAX_HERRAMIENTAS], int tope_herramientas, coordenada_t posicion){
    bool encontrado = false;
    int herramienta_encontrada = HERRAMIENTA_NO_ENCONTRADA;
    int i = 0;
    while(i < tope_herramientas && !encontrado){
        if(herramientas[i].posicion.col == posicion.col && herramientas[i].posicion.fil == posicion.fil){
            herramienta_encontrada = i;
            encontrado = true;
        }
        i++;
    }
    return herramienta_encontrada;
}

//pre: personaje, bombas y tope_bombas son válidos
//post: devuelve true si el personaje cumple lo requerido para desactivar una bomba, false en caso contrario.
bool cumple_condicion_de_desactivar_bomba(personaje_t personaje, bomba_t bombas[MAX_BOMBAS], int tope_bombas){
    return (personaje.energia >= ENERGIA_DESACTIVAR_BOMBA && !(personaje.camuflado) && bomba_a_desactivar(bombas, tope_bombas, personaje.posicion)!= BOMBA_NO_ENCONTRADA) ;
}


//pre: tope bombas es valido.
//post: devuelve cuantas bombas explotaron actualmente.
int cantidad_bombas_explotadas(bomba_t bombas[MAX_BOMBAS], int tope_bombas){
    int contador = 0;
    for(int i = 0; i < tope_bombas; i++){
        if(!(bombas[i].desactivada) && bombas[i].timer == NO_MAS_TIEMPO){
            bombas[i].desactivada = true;
            contador++;
        }
    }
    return contador;
}

//pre:
//post: castea el numero de un int, a unsigned int.
void sacar_signo(int* numero){
	if(*numero < 0){
		*numero = *numero * (-1);
	}
}

//pre: las coordenadas personaje y objeto son válidas.
//post: se fija la distancia manhattan del personaje con otro objeto.
int ver_distancia(coordenada_t personaje, coordenada_t objeto){
    int distancia_col = personaje.col - objeto.col;
    int distancia_fil = personaje.fil - objeto.fil;
    sacar_signo(&distancia_col);
    sacar_signo(&distancia_fil);
    return distancia_col + distancia_fil;
}


//pre: tope familiares es valido.
//post: devuelve la cantidad de familiares que estan a distancia uno de perry.
int cantidad_familiares_cerca(coordenada_t personaje_posicion, familiar_t familiares[MAX_FAMILIARES], int tope_familiares){
    int contador = 0;
    for(int i = 0; i < tope_familiares; i++){
        int distancia = ver_distancia(personaje_posicion, familiares[i].posicion);
        if(distancia <= DISTANCIA_FAMILIARES){
            contador++;
        }
    }
    return contador;
}

//pre: tope bombas tope herramientas tope familiares y tope robots son validos. tope_coordenadas < MAX_COORDENADAS.
//post: carga el vector coordenadas con las coordenadas de todos los objetos.
void cargar_coordenadas(coordenada_t coordenadas[MAX_COORDENADAS], bomba_t bombas[MAX_BOMBAS], int tope_bombas, herramienta_t herramientas[MAX_HERRAMIENTAS], int tope_herramientas, familiar_t familiares[MAX_FAMILIARES], int tope_familiares, coordenada_t personaje_posicion, coordenada_t *robots, int tope_robots){
    int tope_coordenadas = 0;
    for (int i = tope_coordenadas; i < tope_bombas; i++){
        coordenadas[i] = bombas[i].posicion;
    }
    tope_coordenadas = tope_bombas;
    for (int i = tope_coordenadas; i < tope_coordenadas + tope_herramientas; i++){
        coordenadas[i] = herramientas[i - tope_coordenadas].posicion;
    }
    tope_coordenadas += tope_herramientas;
    for(int i = tope_coordenadas + tope_herramientas; i < tope_coordenadas + tope_familiares; i++){
        coordenadas[i] = familiares[i - tope_coordenadas].posicion;
    }
    tope_coordenadas += tope_familiares;
    for(int i = tope_coordenadas; i < tope_coordenadas + tope_robots; i++){
        coordenadas[i] = robots[i - tope_coordenadas];
    }
    tope_coordenadas += tope_robots;
    coordenadas[tope_coordenadas] = personaje_posicion;

}

//pre: tope familiares es valido.
//post: Baja el timer de las bombas, y mueve a los familiares.
void actualizar_movimientos(bomba_t bombas[MAX_BOMBAS], int tope_bombas, familiar_t familiares[MAX_FAMILIARES], int tope_familiares){
    bajar_timer_bombas(bombas, tope_bombas);
    for(int i = 0; i < tope_familiares; i++){
        mover_familiar(&(familiares[i]));
    }
}

//pre: bombas, herramientas, y familiares son arreglos válidos. tope_bombas, tope_herramientas, y tope_familiares son validos.
//post: Se fia si en la posicion actual de perry puede hacer algo con el ambiente, como desactivar bombas, agarrar herramientas o ser descubierto por un familiar.
void actualizar_condiciones_juego(bomba_t bombas[MAX_BOMBAS], int tope_bombas, personaje_t *personaje, herramienta_t herramientas[MAX_HERRAMIENTAS], int *tope_herramientas, familiar_t familiares[MAX_FAMILIARES], int tope_familiares){
    int cant_bombas = cantidad_bombas_explotadas(bombas, tope_bombas);
    (personaje->vida)-= cant_bombas;
    int index_herramienta = herramienta_a_agarrar(herramientas, *(tope_herramientas), personaje->posicion);
    if(cumple_condicion_de_desactivar_bomba(*(personaje), bombas, tope_bombas)){
        int index_bomba = bomba_a_desactivar(bombas, tope_bombas, personaje->posicion);
        if(!(bombas[index_bomba].desactivada)){
            desactivar_bomba(personaje, &(bombas[index_bomba]));
        }
    } else if(index_herramienta != HERRAMIENTA_NO_ENCONTRADA){
        agarrar_herramienta(personaje, herramientas[index_herramienta], *(tope_herramientas));
        eliminar_herramienta(herramientas, tope_herramientas, index_herramienta);
    }
    if(!(personaje->camuflado)){
        int cant_familiares = cantidad_familiares_cerca(personaje->posicion, familiares, tope_familiares);
        personaje->vida -= cant_familiares;
        
    } 
}


//pre: inicializar coordenadas.
//post: guarda memoria para la coordenada de un robot y la inicializa.
void inicializar_robot(coordenada_t* *robots, coordenada_t coordenadas[MAX_COORDENADAS], int tope_coordenadas){
    *robots = malloc(sizeof(coordenada_t));
    **robots = obtener_coordenada_libre(coordenadas, &tope_coordenadas);
}

//pre: cant_robots = juego.cantidad_robots - 1.
//post: agrega unas nuevas coordenadas de un nuevo robot.
void agregar_nuevo_robot(coordenada_t *robots, coordenada_t coordenadas[MAX_COORDENADAS], int tope_coordenadas, int cant_robots){
    *(robots + cant_robots) = obtener_coordenada_libre(coordenadas, &tope_coordenadas);
}

//pre: los arrays coordenadas, bombas, herramientas, familiares y robots son válidos. tope_coordenadas, tope_bombas, tope_herramientas y tope_familiares son validos.
//post: se fija las coordenadas actuales de todos los objetos e inicializa un robot nuevo con coordenadas nuevas y se suma 1 a juego.cantidad_robots.
void hacer_robot(coordenada_t* *robots, int *cantidad_robots, coordenada_t coordenadas[MAX_COORDENADAS], int tope_coordenadas, bomba_t bombas[MAX_BOMBAS], int tope_bombas, herramienta_t herramientas[MAX_HERRAMIENTAS], int tope_herramientas, familiar_t familiares[MAX_FAMILIARES], int tope_familiares, coordenada_t personaje_posicion){
    cargar_coordenadas(coordenadas, bombas, tope_bombas, herramientas, tope_herramientas, familiares, tope_familiares, personaje_posicion, *(robots), *(cantidad_robots));
    if(*(cantidad_robots) == NO_MAS_ROBOTS){
        inicializar_robot(robots, coordenadas, tope_coordenadas);  

    } else if(*(cantidad_robots) > NO_MAS_ROBOTS){
        *robots = realloc(*(robots), sizeof(coordenada_t) * (size_t)(*(cantidad_robots) + 1));
        agregar_nuevo_robot(*(robots), coordenadas, tope_coordenadas, *(cantidad_robots));
        
    }
    *(cantidad_robots)+= 1;
}

//pre: tope es un tope valido.
//post: se fija cuantos robots se encuentran a distancia manhattan dos del personaje.
int cantidad_robots_cerca(coordenada_t personaje_posicion, coordenada_t *robots, int tope){
    int cant_robots = 0;
    for(int i = 0; i < tope; i++){
        int distancia = ver_distancia(personaje_posicion, robots[i]);
        if(distancia <= DISTANCIA_ROBOTS){
            cant_robots++;
        }
    }
    return cant_robots;
}

//pre: 
//post: devuelve la posicion del robot que esta a distancia dos o menos de perry, si no existe devuelve -1.
int encontrar_robot(coordenada_t *robots, int tope, coordenada_t personaje_posicion){
    bool encontrado = false;
    int robot_encontrado = ROBOT_NO_ENCONTRADO;
    int i = 0;
    while(i < tope && !(encontrado)){
        int distancia = ver_distancia(personaje_posicion, robots[i]);
        if(distancia <= DISTANCIA_ROBOTS){
            robot_encontrado = i;
            encontrado = true;
        }
        i++;
    }
    return robot_encontrado;
}

//pre: index_robot_eliminar <= tope - 1.
//post: lleva el elemento en la posicion "index_robot_eliminar" al final y el del final a esa posicion.
void llevar_robot_al_final(coordenada_t *robots, int tope, int index_robot_eliminar){
    coordenada_t aux = robots[tope - 1]; 
    robots[tope - 1] = robots[index_robot_eliminar];
    robots[index_robot_eliminar] = aux;
}

//pre: tope es valido.
//post: copia todos los elementos del vector menos el ultimo.
void copiar_arreglo(coordenada_t *robots, int tope, coordenada_t *robots_aux){            
    for(int i = 0; i < tope - 1; i++){
        robots_aux[i] = robots[i];      
    }
}

//pre: tope es valido.
//post: crea un nuevo puntero aux que guarda memoria para un arreglo de coordenadas_t con tope - 1 elementos. Luego hace que *robots guarde ese arreglo.
void destruir_robot(coordenada_t* *robots, int tope){
    coordenada_t *robots_aux = malloc(sizeof(coordenada_t) * (size_t)(tope - 1));
    copiar_arreglo(*(robots), tope, robots_aux);
    free(*robots);
    *robots = robots_aux;     
}

//pre: cantidad_robots solo puede ser >= 1. encontrar_robot va a dar un index existente.
//post: el personaje pierde una vida si esta camuflado, si no tiene energia para pelear tambien y se destruye el robot y si pelea tambien se destruye.
void pelear_con_un_robot(personaje_t *personaje, coordenada_t* *robots, int *cantidad_robots){
    if(personaje->camuflado){
        int cant_robots = cantidad_robots_cerca(personaje->posicion, *robots, *(cantidad_robots));
        personaje->vida -= cant_robots;

    } else{
        if(*(cantidad_robots) > 1){
            int index_robot = encontrar_robot(*robots, *(cantidad_robots), personaje->posicion);
            llevar_robot_al_final(*robots, *(cantidad_robots), index_robot);
            destruir_robot(robots, *(cantidad_robots));
        } else{
            free(*robots);
        }
        if(personaje->energia >= ENERGIA_DESTRUIR_ROBOT){
            personaje->energia -= ENERGIA_DESTRUIR_ROBOT;
        } else {
            personaje->vida--;
        }
        *(cantidad_robots)-= 1;
    }
}

void realizar_jugada(juego_t* juego, char accion){
    realizar_accion(&(juego->perry), accion, &(juego->movimientos));
    actualizar_movimientos(juego->bombas, juego->tope_bombas, juego->familiares, juego->tope_familiares);
    coordenada_t coordenadas[MAX_COORDENADAS];
    int tope_coordenadas = juego->tope_bombas + juego->tope_herramientas + juego->tope_familiares + TOPE_PERRY;
    if(encontrar_robot(juego->robots, juego->cantidad_robots, juego->perry.posicion) != ROBOT_NO_ENCONTRADO){
        pelear_con_un_robot(&(juego->perry), &(juego->robots), &(juego->cantidad_robots));
    }
    actualizar_condiciones_juego(juego->bombas, juego->tope_bombas, &(juego->perry), juego->herramientas, &(juego->tope_herramientas), juego->familiares, juego->tope_familiares);
    if(juego->movimientos %  CANT_CREAR_ROBOTS == 0){
        hacer_robot(&(juego->robots), &(juego->cantidad_robots), coordenadas, tope_coordenadas, juego->bombas, juego->tope_bombas, juego->herramientas, juego->tope_herramientas, juego->familiares, juego->tope_familiares, juego->perry.posicion); 
        
    }
    
}

//pre: MAX_FIL Y MAX_COL > 0
//post: crea una matriz [MAX FIL] X [MAS COL].
void crear_tablero(char tablero[MAX_FIL][MAX_COL]){
    for(int i = 0; i < MAX_FIL; i++){
        for (int j = 0; j < MAX_COL; j++){
            tablero[i][j] = VACIO;
        }
    }
}

//pre: coordenada es valido.
//post: agrega un objeto al tablero.
void agregar_objeto_al_tablero(char tablero[MAX_FIL][MAX_COL], char objeto, coordenada_t coordenada){
    tablero[coordenada.fil][coordenada.col] = objeto;
}

//pre:  tope y bombas[i].posicion son validos.          
//post: agrega el vector de bombas al tablero.
void agregar_bombas_al_tablero(char tablero[MAX_FIL][MAX_COL], char objeto, bomba_t bombas[MAX_BOMBAS] , int tope) {
    for(int i = 0; i < tope; i++){
        if(!(bombas[i].desactivada) && bombas[i].timer > NO_MAS_TIEMPO){
            agregar_objeto_al_tablero(tablero, objeto, bombas[i].posicion);
        }
    }
}


//pre: tope y herramientas[i].posicion son validos
//post: agrega el vector de herramientas al tablero.
void agregar_herramientas_al_tablero(char tablero[MAX_FIL][MAX_COL], herramienta_t herramientas[MAX_HERRAMIENTAS], int tope){
    for(int i = 0; i < tope; i++){
        herramienta_t herramienta = herramientas[i];
        agregar_objeto_al_tablero(tablero, herramienta.tipo, herramienta.posicion);
    }
}

//pre: tope y familiares[i].posicion son validos
//post: agrega el vector de familiares al tablero.
void agregar_familiares_al_tablero(char tablero[MAX_FIL][MAX_COL], familiar_t familiares[MAX_FAMILIARES] , int tope){
    for(int i = 0; i < tope; i++){
        familiar_t familiar = familiares[i];
        agregar_objeto_al_tablero(tablero, familiar.inicial_nombre, familiar.posicion);
    }
}

//pre: tope y robots[i] son validos
// post: agrega el vector de robots al tablero.
void agregar_robots_al_tablero(char tablero[MAX_FIL][MAX_COL], coordenada_t *robots, int tope){
    for(int i = 0; i < tope; i++){
        agregar_objeto_al_tablero(tablero, ROBOTS, robots[i]);
    }
}

//pre: crear el tablero.
//post: imprime en la terminal el tablero y lo que se le haya agregado.
void imprimir_tablero(char tablero[MAX_FIL][MAX_COL]){
    for(int i = 0; i < MAX_FIL; i++){
        printf("\n");
        for (int j = 0; j < MAX_COL; j++){
            printf("%c", tablero[i][j]);
        }
    }
}

//pre: _
//post: imprime la posicion de las bombas , el timer correspondiente a cada una y si estan desactivadas.
void imprimir_estadisticas_bombas(char tablero[MAX_FIL][MAX_COL], bomba_t bombas[MAX_BOMBAS], int tope){
    for(int i = 0; i < tope; i++){
        printf("Bomba[%i]:", i + 1);
        printf(" fil: %i", bombas[i].posicion.fil);
        printf(" col: %i", bombas[i].posicion.col);
        printf(" tiempo: %i", bombas[i].timer);
        if(bombas[i].desactivada){
            printf(" Desactivada ");
        }
        printf("\n");
    }
}


void imprimir_terreno(juego_t juego){
    char tablero[MAX_FIL][MAX_COL];
    crear_tablero(tablero);   
    system("clear");
    agregar_bombas_al_tablero(tablero, BOMBA, juego.bombas, juego.tope_bombas);
    agregar_herramientas_al_tablero(tablero,juego.herramientas, juego.tope_herramientas);
    agregar_familiares_al_tablero(tablero, juego.familiares, juego.tope_familiares);
    agregar_objeto_al_tablero(tablero, PERRY, juego.perry.posicion);
    agregar_robots_al_tablero(tablero, juego.robots, juego.cantidad_robots);
    imprimir_tablero(tablero);
    printf("\nVidas: %i \n",juego.perry.vida);
    printf("Energia: %i \n",juego.perry.energia);
    printf("Cantidad de Movimientos: %i \n", juego.movimientos);
    printf("CANTIDAD ROBOTS: %i \n", juego.cantidad_robots);
    imprimir_estadisticas_bombas(tablero, juego.bombas, juego.tope_bombas);
    if(juego.perry.camuflado){
        printf("Perry esta camuflado\n");
    } else{
        printf("Perry no esta camuflado\n");
    }
}

//pre: en base a un vector bomba_t bombas[MAX_BOMBAS], que no este vacio.
//post: se fija todas las bombas.desactivadas del vector devuelve true si estan todas desactivadas o false en caso contrario.
bool estan_todas_desactivadas(bomba_t bombas[MAX_BOMBAS], int tope_bombas){
    bool hay_alguna_activa = false;
    int i = 0;
    while(i < tope_bombas && !hay_alguna_activa){
        if(!(bombas[i].desactivada)){
            hay_alguna_activa = true;
        } 
        i++;
    }
    return !(hay_alguna_activa);
}

int estado_juego(juego_t juego){
    if(juego.perry.vida <= NO_MAS_VIDAS ){
        if(juego.cantidad_robots != NO_MAS_ROBOTS){
            free(juego.robots);


        }
        return TERMINAR_JUEGO_PERDER;
    } else if((estan_todas_desactivadas(juego.bombas, juego.tope_bombas))){
        if(juego.cantidad_robots != NO_MAS_ROBOTS){
            free(juego.robots);
        }
        return TERMINAR_JUEGO_GANAR;
    } else 
        return SEGUIR_JUGANDO;
}

