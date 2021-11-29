#ifndef GAMELOOP_H_
#define GAMELOOP_H_

/*  gameloop contiene funciones de jugadas y el do...while principal que ejecuta el juego en si.
*/

/* Ejecuta el juego */
int gameloop();
    /* solo retorna 0
        tal vez se necesite retornar un valor de error en el futuro, pero hasta ahora no.
    */

/* Verifica un disparo y retorna el resultado. */
int validateShot(int player, int fila, int col);
    /* Parametros:
        -player: 0 o 1
        -fila/columna: 0 a 9
       ret:
        0 = fallo
        1 = acierto
        -1 = fuera del tablero
        -2 = celda ya disparada
    */

/* Efectua el disparo, es decir actualiza los tableros y barcos dependiendo del resultado del disparo.
   NO revisa si el disparo es valido. */
void shoot(int player, int fila, int col);
    /* Parametros:
        -player: 0 o 1
        -fila/columna: 0 a 9
    */

/* Llama a la funcion de disparo correspondiente a la dificultad */
int ai_shoot(int *i, int *j);
    /* Parametros:
        -i: puntero a un entero en el que se guarda la x
        -j: puntero a un entero en el que se guarda la y
       ret:
        0: miss
        1: hit
    */

/* Verifica cual jugador es el ganador */
int winner();
    /* ret:
        0: player 1
        1: player 2
        -1: empate
    */

/* Detiene ejecucion por el tiempo determinado */
void delay(float seconds);

#endif // GAMELOOP_H_
