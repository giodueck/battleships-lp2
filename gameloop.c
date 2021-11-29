#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gameloop.h"
#include "ui.h"
#include "board.h"
#include "rand.h"
#include "hunt_target.h"
#include "prob_density.h"

int gamemode;   //0: PvE, 1: EvE
int difficulty; //0 (random), 1 (HTA), 2 (PDA)
int limite_disparos;    //entre 1 y 100
int turn;               //toma valores de PLAYER_n de board.h

//    //copy-paste de board.c para poder usarse en este archivo
//struct ship     //estructura que guarda informaciones sobre cada barco
//{
//    int size, len;
//    int orient;     //horizontal o vertical
//    int live;       //0 o 1 dependiendo si fue hundido o no
//    struct coord
//    {
//        int x, y;
//    } * pos;        //puntero a coordenadas de cada celda ocupada;
//};

extern struct ship* fleet_1;
extern struct ship* fleet_2;
extern int fleetsize_n;

int gameloop()
{
    /* Ejecuta el juego */

    int game_over = FALSE;  //para la condicion de fin del do...while
    int disparos = 0;       //cuenta los disparos del usuario
    int disparos_ai = 0;    //  "     "     "      "  programa
    extern int lives_1;
    extern int lives_2;
    int ships_st, ships_end;    //sirven para notificar al jugador si se hunde un barco
    int hit = FALSE;        //indica si el disparo resulto en un acierto
    int sunk;               //guarda el resultado de mark_sunk para la impresion del mensaje de barco hundido
    int x, y;

    /* Configuracion de la partida */
    ui_settings();
    seed();
    lives_1 = fleetsize_n;
    lives_2 = fleetsize_n;

    /* Preparacion de tableros */
    setup_board();

    /* Gameloop principal */
    if (gamemode == 0)
    {
        if (difficulty == 1)    //inicializacion de tableros para los algoritmos
            init_HT();
        else if (difficulty == 2)
            init_PD();

        if (difficulty == 2)    //colocacion de barcos para PLAYER_2
            setup_board_PD();
        else
            setup_board_rand();

        printf("\n\n  ## INICIO DE PARTIDA ##\n");
        delay(1);
        do
        {
            if (turn == PLAYER_1)   //usuario, llama las funciones de ui
            {
                ships_st = count_ships(PLAYER_2);
                printf("\n<!>\tSu turno!\n");
                brief(disparos, disparos_ai);
                hit = shoot_ui();
                ships_end = count_ships(PLAYER_2);

                if (ships_end < ships_st)   //si se hunde un barco
                {
                    lives_2--;
                    sunk = mark_sunk(PLAYER_2);

                    switch (sunk)
                    {
                    case 0:
                        printf("\n<!>\tFragata enemiga hundida!\n");
                        break;
                    case 1:
                        printf("\n<!>\tDestructor enemigo hundido!\n");
                        break;
                    case 2:
                        printf("\n<!>\tAcorazado enemigo hundido!\n");
                        break;
                    case 3:
                        printf("\n<!>\tSubmarino enemigo hundido!\n");
                        break;
                    case 4:
                        printf("\n<!>\tPortaviones enemigo hundido!\n");
                        break;
                    default:
                        break;
                    }
                }

                if (!hit)
                {
                    turn = PLAYER_2;
                    disparos++;
                }

                //delay(2);
            } else if (turn == PLAYER_2)    //programa, llama funciones sin ui
            {
                ships_st = count_ships(PLAYER_1);
                printf("\n<!>\tTurno del oponente.\n");
                delay(1);

                hit = ai_shoot(&x, &y);
                ai_shot_msg(x, y, hit);
                ships_end = count_ships(PLAYER_1);
                //delay(2);

                if (ships_end < ships_st)
                {
                    lives_1--;
                    sunk = mark_sunk(PLAYER_1);
                    mark_sunk_PD(sunk);

                    switch (sunk)
                    {
                    case 0:
                        printf("\n<!>\tFragata aliada hundida!\n");
                        break;
                    case 1:
                        printf("\n<!>\tDestructor aliado hundido!\n");
                        break;
                    case 2:
                        printf("\n<!>\tAcorazado aliado hundido!\n");
                        break;
                    case 3:
                        printf("\n<!>\tSubmarino aliado hundido!\n");
                        break;
                    case 4:
                        printf("\n<!>\tPortaviones aliado hundido!\n");
                        break;
                    default:
                        break;
                    }
                    delay(1);
                }

                if (!hit)
                {
                    turn = PLAYER_1;
                    disparos_ai++;
                }
            }

            if (lives_1 == 0)
            {
                game_over = TRUE;
            } else if (lives_2 == 0)
            {
                game_over = TRUE;
            }
                                //ambos jugadores deben alcamzar el limite para terminar
        } while (!game_over && (disparos < limite_disparos || disparos_ai < limite_disparos));

        game_over_msg(winner());
    } //else if (gamemode == 1)

    return 0;
}

int validateShot(int player, int fila, int col)
{
    /* Verifica un disparo y evalua el resultado. */

	int res = 0;    //resultado puede ser un error o indicar si se dio al objetivo

	struct ship* pship;
	int enemy[10][10];

	if (player == PLAYER_1)
    {
        pship = fleet_2;                //informacion sobre la flota enemiga
        extern int enemyb_1[10][10];
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)    //tableros a actualizarse
            {
                enemy[i][j] = enemyb_1[i][j];   //tablero en el que se dispara
            }
        }
    } else if (player == PLAYER_2)
    {
        pship = fleet_1;
        extern int enemyb_2[10][10];
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                enemy[i][j] = enemyb_2[i][j];
            }
        }
    }

    if ((fila < 0 || fila >= 10) || (col < 0 || col >= 10))
    {
        res = -1;
    } else if (enemy[fila][col] == HIT || enemy[fila][col] == MISS || enemy[fila][col] == SUNK)
    {
        res = -2;
    } else  //se itera el puntero que guarda las posiciones enemigas
    {
        for (int i = 0; i < fleetsize_n; i++)   //itera las celdas ocupadas por los barcos para revisar si se acierta el disparo
        {
            for (int j = 0; j < (pship[i]).len; j++) //j < (longitud del barco)
            {
                if (pship[i].pos[j].x == fila && pship[i].pos[j].y == col)
                {
                    res = 1;
                    break;
                }
            }
            if (res == 1) break;
        }
        //si no se detecta un acierto res queda = 0
    }

    /* res: 0 = fallo
            1 = acierto
            -1 = fuera del tablero
            -2 = celda ya disparada
    */
    return res;
}

void shoot(int player, int fila, int col)
{
    /* Efectua el disparo, es decir actualiza los tableros y barcos dependiendo del resultado del disparo.
       NO revisa si el disparo es valido. */

    int hit = FALSE;

    if (player == PLAYER_1)
    {
        extern int enemyb_1[10][10];
        extern int fleetb_2[10][10];

        if (fleetb_2[fila][col] != AGUA)   //si en la posicion hay un barco
        {           //usa fleet_2 para revisar los barcos enemigos
            for (int i = 0; i < fleetsize_n; i++)   //itera las celdas ocupadas por los barcos para revisar si se acierta el disparo
            {
                for (int j = 0; j < fleet_2[i].len; j++) //j < (longitud del barco)
                {
                    if (fleet_2[i].pos[j].x == fila && fleet_2[i].pos[j].y == col)
                    {
                        (fleet_2[i].live)--;
                        hit = TRUE;
                        break;
                    }
                }
                if (hit == TRUE) break;
            }
        }

        if (hit == TRUE)
        {
            enemyb_1[fila][col] = HIT;
            fleetb_2[fila][col] = HIT;
        } else if (hit == FALSE)
        {
            enemyb_1[fila][col] = MISS;
            fleetb_2[fila][col] = MISS;
        }
    } else if (player == PLAYER_2)
    {
        extern int enemyb_2[10][10];
        extern int fleetb_1[10][10];

        if (fleetb_1[fila][col] != AGUA)   //si en la posicion hay un barco
        {           //usa fleet_1 para revisar los barcos enemigos
            for (int i = 0; i < fleetsize_n; i++)   //itera las celdas ocupadas por los barcos para revisar si se acierta el disparo
            {
                for (int j = 0; j < fleet_1[i].len; j++) //j < (longitud del barco)
                {
                    if (fleet_1[i].pos[j].x == fila && fleet_1[i].pos[j].y == col)
                    {
                        (fleet_1[i].live)--;
                        hit = TRUE;
                        break;
                    }
                }
                if (hit == TRUE) break;
            }
        }

        if (hit == TRUE)
        {
            enemyb_2[fila][col] = HIT;
            fleetb_1[fila][col] = HIT;
        } else if (hit == FALSE)
        {
            enemyb_2[fila][col] = MISS;
            fleetb_1[fila][col] = MISS;
        }
    }
}

int ai_shoot(int *i, int *j)
{
    /* Llama a la funcion de disparo correspondiente a la dificultad */

    if (difficulty == 0)
    {
        return shoot_rand(i, j);
    } else if (difficulty == 1)
    {
        return HTshoot(i, j);
    } else
    {
        return PDshoot(i, j);
    }
}

int winner()
{
    /* Verifica cual jugador es el ganador */

    /*
    condiciones de victoria:
        -flota enemiga hundida
        -limite (mayor cantidad de barcos vivos)
    */
    extern int lives_1;
    extern int lives_2;

    if (lives_1 == 0)
        return PLAYER_2;
    else if (lives_2 == 0)
        return PLAYER_1;
    else
    {
        if (lives_1 > lives_2)
            return PLAYER_1;
        else if (lives_2 > lives_1)
            return PLAYER_2;
    }

    return -1;
}

void delay(float seconds)
{
    /* Detiene ejecucion por el tiempo determinado */

    int ms = seconds*1000;  //conversion a milisegundos
    int start;

    start = clock();    //clock() retorna el tiempo desde el inicio de ejecucion del programa

    while (clock() < start + ms);   //se ejecuta hasta llegar al tiempo especificado
}
