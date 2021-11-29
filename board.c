#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "prob_density.h"

/*
    Los tableros contienen informacion sobre cada celda. Aqui se guardan datos sobre los
    barcos vivos y muertos y disparos acertados y fallidos. Todas las demas estructuras de datos
    y todas las funciones usan o se basan en estas matrices.
    Tableros:
        -fleetb (fleet board) corresponde a la flota del jugador.
        -enemyb (enemy board) corresponde al tablero en el que se realizan los disparos
            al enemigo.
    Cada jugador tiene un tablero de cada variante, el numero _n da a conocer a que juagdor
        corresponde cada tablero.
*/

int fleetb_1[10][10] = {0}; //se inicializan en 0, es decir solo contienen AGUA de board.h
int fleetb_2[10][10] = {0};
int enemyb_1[10][10] = {0};
int enemyb_2[10][10] = {0};

/*
    Barcos:
        0-fragatas: 1 celda
        1-destructores: 2 celdas
        2-acorazados: 3 celdas
        3-submarinos: 3 celdas
        4-portaviones: 4 celdas
    El array fleetsize almacena la configuracion inicial de cantidad de barco de cada tipo por jugador.
        Esta ordenado por tamanho de bote de menor a mayor, fleetsize[0] cuenta las fragatas.
    Los arrays de punteros fleet almacenan punteros a memoria reservada con malloc/realloc que guardan
        las coordenadas de los botes de la flota del jugador correspondiente. Usan fleetsize para
        almacenar la longitud de estos bloques de botes.
    fleetsize_n contiene el tamanho total de las flotas.
*/
int fleetsize[5], fleetsize_n;

/*  Usar un struct para guardar informaciones de los barcos facilita el acceso a informaciones
    individuales sobre un barco en especifico.

    Se usa un puntero a struct coord para almacenar posiciones de cada celda porque la
    longitud de un barco cualquiera es variable (1-4). Cada struct coord contiene las
    coordenadas de una celda
*/

//struct ship     //estructura que guarda informaciones sobre cada barco
//{
//    int size, len;
//    int orient;     //horizontal o vertical
//    int live;       //0 a len segun las celdas que no fueron disparadas
//    struct coord
//    {
//        int x, y;
//    } * pos;        //puntero a coordenadas de cada celda ocupada;
//};

//indice de un barco determinado sera: n de barcos anteriores + cantidad de barcos del tipo - 1
struct ship* fleet_1 = NULL;
struct ship* fleet_2 = NULL;

//cantidad de barcos vivos por jugador
int lives_1;
int lives_2;

void print_board(int player, int tipo)
{
    /* Imprime el tablero especificado para el jugador player.
       tipo (de tablero): 0 o 1, flota o enemigo
    */
    int tablero[10][10];
    //tablero se asigna aqui para evitar usar if en la impresion misma
    if (player == PLAYER_1)
    {
        if (tipo == 0)
        {
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    tablero[i][j] = fleetb_1[i][j];
                    if (tablero[i][j] == 0) //print_board imprime el tablero con los valores de board.h
                        tablero[i][j] = ' ';//excepto si es AGUA o 0. En ese caso imprime ' '
                }
            }
        } else if (tipo == 1)
        {
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    tablero[i][j] = enemyb_1[i][j];
                    if (tablero[i][j] == 0)
                        tablero[i][j] = ' ';
                }
            }
        }
    } else if (player == PLAYER_2)
    {
        if (tipo == 0)
        {
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    tablero[i][j] = fleetb_2[i][j];
                    if (tablero[i][j] == 0)
                        tablero[i][j] = ' ';
                }
            }
        } else if (tipo == 1)
        {
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    tablero[i][j] = enemyb_2[i][j];
                    if (tablero[i][j] == 0)
                        tablero[i][j] = ' ';
                }
            }
        }
    }

    /* Impresion simple */
    /*
    printf("\n\n");
    if (player == PLAYER_1)
    {
        if (tipo == 0)  //flota
        {
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    if (fleetb_1[i][j] == 0)
                        printf(" ~");
                    else printf(" %c", fleetb_1[i][j]);
                    if (j == 9) printf("\n");
                }
            }
        } else if (tipo == 1) //enemigo
        {
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    if (enemyb_1[i][j] == 0)
                        printf(" ~");
                    else printf(" %c", enemyb_1[i][j]);
                    if (j == 9) printf("\n");
                }
            }
        }
    } else if (player == PLAYER_2)
    {
        if (tipo == 0)  //flota
        {
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    if (fleetb_2[i][j] == 0)
                        printf(" ~");
                    else printf(" %c", fleetb_2[i][j]);
                    if (j == 9) printf("\n");
                }
            }
        } else if (tipo == 1) //enemigo
        {
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    if (enemyb_2[i][j] == 0)
                        printf(" ~");
                    else printf(" %c", enemyb_2[i][j]);
                    if (j == 9) printf("\n");
                }
            }
        }
    }
    printf("\n");
    */

    /* Impresion regular */
    for (int i =0; i < 10; i++)
    {
        for (int j =0; j < 10; j++)
        {
            if (i == 0 && j == 0)   //coordenadas
            {
                //        -   1   2   3   4   5   6   7   8   9  10
                printf("\n\n");
                printf("+---+---+---+---+---+---+---+---+---+---+---+\n");
                printf("|   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |10 |\n");
                printf("+---+---+---+---+---+---+---+---+---+---+---+\n");
            }

            if (j == 0) //primera celda de una linea
            {
                if (i == 9) //fila 10
                    printf("|%d | %c |", i+1, tablero[i][j]);
                else        //filas 1-9
                    printf("| %d | %c |", i+1, tablero[i][j]);
            } else if (j == 9)  //ultima celda de una linea
            {
                printf(" %c |\n", tablero[i][j]);
                printf("+---+---+---+---+---+---+---+---+---+---+---+\n");
            } else  //celda del medio
            {
                printf(" %c |", tablero[i][j]);
            }
        }
    }
}

void reset()
{
    /* Resetea todos los tableros a {0} */
    extern int aux_board[10][10];

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            fleetb_1[i][j] = 0;
            fleetb_2[i][j] = 0;
            enemyb_1[i][j] = 0;
            enemyb_2[i][j] = 0;
            aux_board[i][j] = 0;
        }
    }
}

void set_fleetsize(int n_fra, int n_des, int n_aco, int n_sub, int n_por, int n_tot)
{
    /* Permite configurar la cantidad de cada tipo de barco que tendra cada jugador.
       Guarda cantidades por tipo en fleetsize[] y total en fleetsize_n.
     */

    fleetsize_n = n_tot;

    fleetsize[0] = n_fra;
    fleetsize[1] = n_des;
    fleetsize[2] = n_aco;
    fleetsize[3] = n_sub;
    fleetsize[4] = n_por;

    fleet_1 = malloc(sizeof(struct ship)*n_tot);
    fleet_2 = malloc(sizeof(struct ship)*n_tot);

    for (int i = 0; i < n_tot; i++)
    {
        fleet_1[i].pos = NULL;
        fleet_2[i].pos = NULL;
    }
}

int place_ship(int player, int ship_size, int ship_n, int orient, int x, int y)
{
    /* Ubica un barco sobre el tablero y genera un struct ship */

    /* Parametros:
        player: 0 o 1. En modo PvE, 0 es el jugador, 1 es la computadora
        ship_size: 0 a 4
        ship_n: 0 a fleetsize[ship_size] - 1, un conteo de barcos del mismo tipo
        orient: 0 a 1
        x,y: coordenadas, 0 a 9
    */

    int index, aux = 0;
    struct ship temp;
    int error = 0;
    int valor;  //valor para cada tipo de barco definido en board.h, se guarda en la matriz tablero de flota

    extern int aux_board[10][10];

    for (int i = 0; i < ship_size; i++) //suma la cantidad de botes de menores tamanhos,
        aux += fleetsize[i];            // esos botes seran guardados en menores indices

    index = aux + ship_n;               //se guarda la posicion en la que se guardaria el barco en fleet_1/2

    /* Se revisa si las coordenadas de entrada producen errores*/
    if (ship_size == 0 || ship_size == 1 || ship_size == 2)
        aux = ship_size+1;
    else aux = ship_size;   //aux toma el valor de la longitud del barco. Posibilita usar un "for" para todos los casos

    if (orient == HORIZONTAL)   //asigna un valor a error si existe un error, significado de cada valor al final
    {
        for (int j = 0; j < aux; j++)
        {
            if (y+j > 9)
            {
                error = -2;
                break;
            }else if (player == PLAYER_1)
            {
                if (fleetb_1[x][y+j] != 0)
                {
                    error = -1;
                    break;
                }
            } else if (player == PLAYER_2)
            {
                if (fleetb_2[x][y+j] != 0)
                {
                    error = -1;
                    break;
                }
            } else
            {
                if (aux_board[x][y+j] != 0)
                {
                    error = -1;
                    break;
                }
            }
        }
    } else if (orient == VERTICAL)
    {
        for (int i = 0; i < aux; i++)
        {
            if (x+i > 9)
            {
                error = -2;
                break;
            }else if (player == PLAYER_1)
            {
                if (fleetb_1[x+i][y] != 0)
                {
                    error = -1;
                    break;
                }
            } else if (player == PLAYER_2)
            {
                if (fleetb_2[x+i][y] != 0)
                {
                    error = -1;
                    break;
                }
            } else
            {
                if (aux_board[x+i][y] != 0)
                {
                    error = -1;
                    break;
                }
            }
        }
    }

    if (!error)
    {
        /* asignacion del struct ship temp */
        temp.size = ship_size;
        temp.len = aux;
        temp.pos = malloc(sizeof(struct coord)*temp.len);
        temp.orient= orient;
        temp.live = temp.len;
        for (int i = 0; i < temp.len; i++)
        {
            if (orient == HORIZONTAL)
            {
                (temp.pos+i)->x = x;
                (temp.pos+i)->y = y+i;
            }
            else if (orient == VERTICAL)
            {
                (temp.pos+i)->x = x+i;
                (temp.pos+i)->y = y;
            }
        }

        /* actualizacion de la matriz tablero de flota */
        switch (ship_size)  //asignacion de valor
        {
        case 0:
            valor = FRAG;
            break;
        case 1:
            valor = DEST;
            break;
        case 2:
            valor = ACOR;
            break;
        case 3:
            valor = SUBM;
            break;
        case 4:
            valor = PORT;
            break;
        }

        if (player == PLAYER_1)     //asignacion de valor a las coordenadas correspondientes
        {
            fleet_1[index] = temp;
            if (orient == HORIZONTAL)
            {
                for (int j = 0; j < aux; j++)
                    fleetb_1[x][y+j] = valor;
            } else if (orient == VERTICAL)
            {
                for (int i = 0; i < aux; i++)
                    fleetb_1[x+i][y] = valor;
            }
        } else if (player == PLAYER_2)
        {
            fleet_2[index] = temp;
            if (orient == HORIZONTAL)
            {
                for (int j = 0; j < aux; j++)
                    fleetb_2[x][y+j] = valor;
            } else if (orient == VERTICAL)
            {
                for (int i = 0; i < aux; i++)
                    fleetb_2[x+i][y] = valor;
            }
        } else
        {
            free(temp.pos);
            if (orient == HORIZONTAL)
            {
                for (int j = 0; j < aux; j++)
                    aux_board[x][y+j] = valor;
            } else if (orient == VERTICAL)
            {
                for (int i = 0; i < aux; i++)
                    aux_board[x+i][y] = valor;
            }
        }

        return 0;

    } else return error;
    //error = -1 -> solapamiento de barcos
    //error = -2 -> fuera del tablero

    return -3;  //error que no beberia ocurrir, la funcion no deberia llegar aqui
}

int count_ships(int player)
{
    /* Cuenta la cantidad de barcos vivos en la flota del jugador */

    int ships = 0;

    if (player == PLAYER_1)
    {
        for (int i = 0; i < fleetsize_n; i++)
        {
            if (fleet_1[i].live != FALSE) ships++;
        }
    } else if (player == PLAYER_2)
    {
        for (int i = 0; i < fleetsize_n; i++)
        {
            if (fleet_2[i].live != FALSE) ships++;
        }
    }

    return ships;
}

int count_ship_type(int player, int type)
{
    /* Cuenta la cantidad de barcos vivos del tipo type en la flota del jugador */

    int ships = 0;
    int i;

    if (player == PLAYER_1)
    {
        if (type == 0)
        {
            ships = fleetsize[0];
            for (i = 0; fleet_1[i].size == type; i++)
            {
                if (fleet_1[i].live == 0)
                    ships--;
            }
        } else if (type == 1)
        {
            ships = fleetsize[1];
            for (i = fleetsize[0]; fleet_1[i].size == type; i++)
            {
                if (fleet_1[i].live == 0)
                    ships--;
            }
        } else if (type == 2)
        {
            ships = fleetsize[2];
            for (i = fleetsize[0] + fleetsize[1]; fleet_1[i].size == type; i++)
            {
                if (fleet_1[i].live == 0)
                    ships--;
            }
        } else if (type == 3)
        {
            ships = fleetsize[3];
            for (i = fleetsize[0] + fleetsize[1] + fleetsize[2]; fleet_1[i].size == type; i++)
            {
                if (fleet_1[i].live == 0)
                    ships--;
            }
        } else if (type == 4)
        {
            ships = fleetsize[4];
            for (i = fleetsize[0] + fleetsize[1] + fleetsize[2] + fleetsize[3]; fleet_1[i].size == type; i++)
            {
                if (fleet_1[i].live == 0)
                    ships--;
            }
        }
    } else if (player == PLAYER_2)
    {
        if (type == 0)
        {
            ships = fleetsize[0];
            for (i = 0; fleet_2[i].size == type; i++)
            {
                if (fleet_2[i].live == 0)
                    ships--;
            }
        } else if (type == 1)
        {
            ships = fleetsize[1];
            for (i = fleetsize[0]; fleet_2[i].size == type; i++)
            {
                if (fleet_2[i].live == 0)
                    ships--;
            }
        } else if (type == 2)
        {
            ships = fleetsize[2];
            for (i = fleetsize[0] + fleetsize[1]; fleet_2[i].size == type; i++)
            {
                if (fleet_2[i].live == 0)
                    ships--;
            }
        } else if (type == 3)
        {
            ships = fleetsize[3];
            for (i = fleetsize[0] + fleetsize[1] + fleetsize[2]; fleet_2[i].size == type; i++)
            {
                if (fleet_2[i].live == 0)
                    ships--;
            }
        } else if (type == 4)
        {
            ships = fleetsize[4];
            for (i = fleetsize[0] + fleetsize[1] + fleetsize[2] + fleetsize[3]; fleet_2[i].size == type; i++)
            {
                if (fleet_2[i].live == 0)
                    ships--;
            }
        }
    }

    return ships;
}

int mark_sunk(int player)
{
    /* Marca barcos hundidos para distinguirlos de los barcos disparados pero vivos */

    extern struct PDcell PDboard[10][10];
    extern int PDactive;

    if (player == PLAYER_1)
    {
        for (int i = 0; i < fleetsize_n; i++)
        {
            if (fleet_1[i].live == FALSE)
            {
                if (fleetb_1[fleet_1[i].pos[0].x][fleet_1[i].pos[0].y] == HIT)
                {
                    for (int j = 0; j < fleet_1[i].len; j++)
                    {
                        fleetb_1[fleet_1[i].pos[j].x][fleet_1[i].pos[j].y] = SUNK;
                        enemyb_2[fleet_1[i].pos[j].x][fleet_1[i].pos[j].y] = SUNK;

                        if (PDactive)
                            PDboard[fleet_1[i].pos[j].x][fleet_1[i].pos[j].y].state = SUNK;
                    }
                    return fleet_1[i].size;
                }
            }
        }
    } else if (player == PLAYER_2)
    {
        for (int i = 0; i < fleetsize_n; i++)
        {
            if (fleet_2[i].live == FALSE)
            {
                if (fleetb_2[fleet_2[i].pos[0].x][fleet_2[i].pos[0].y] == HIT)
                {
                    for (int j = 0; j < fleet_2[i].len; j++)
                    {
                        fleetb_2[fleet_2[i].pos[j].x][fleet_2[i].pos[j].y] = SUNK;
                        enemyb_1[fleet_2[i].pos[j].x][fleet_2[i].pos[j].y] = SUNK;
                    }
                    return fleet_2[i].size;
                }
            }
        }
    }

    return -1;
}
