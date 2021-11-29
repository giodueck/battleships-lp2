#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rand.h"
#include "board.h"
#include "gameloop.h"

void seed()
{
    /* Cambia la semilla del generador para que sea basado en el reloj */
    srand(time(NULL));  //NULL hace que time retorne la hora actual
}

int gen_num(int lower, int upper)
{
    /* Genera un numero entre lower y upper, inclusivo */

    return ((rand() % (upper-lower+1)) + lower);
}

void setup_board_rand()
{
    /* Coordina llamadas de place_ship() para el jugador aleatorio */

    extern int fleetsize[5];
    int error;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < fleetsize[i]; j++)
        {
            do
            {
                error = place_ship(PLAYER_2, i, j, gen_num(0, 1), gen_num(0, 9), gen_num(0, 9));
            } while (error != 0);
        }
    }
}

int shoot_rand(int *i, int *j)
{
    /* Llama a validateShot() y shoot() para el programa usando valores aleatorios */
    int res = 0;
    int fila, col;

    do
    {
        fila = gen_num(0, 9);
        col = gen_num(0, 9);

        res = validateShot(PLAYER_2, fila, col);
    } while (res < 0);

    *i = fila;
    *j = col;

    shoot(PLAYER_2, fila, col);
    return res;
}
