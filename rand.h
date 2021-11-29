#ifndef RAND_H_
#define RAND_H_

/*
    rand contiene funciones relacionadas a generacion de numeros.
    Por ahora tambien contiene a funciones de jugadas aleatorias, que en el
    futuro se reemplazaran por funciones mas inteligentes en un archivo separado.
*/

/* Cambia la semilla del generador para que sea basado en el reloj */
void seed();

/* Genera un numero entre lower y upper, inclusivo */
int gen_num(int lower, int upper);
    /* Parametros:
        -lower: limite inferior (inclusivo)
        -upper: limite superior (inclusivo)
    */

/* Coordina llamadas de place_ship() para el jugador aleatorio */
void setup_board_rand();

/* Llama a validateShot() y shoot() para el programa usando valores aleatorios */
int shoot_rand(int *i, int *j);
    /* ret:
        0: fallo
        1: acierto
    */

#endif // RANDOM_H_
