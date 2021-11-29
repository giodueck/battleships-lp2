#ifndef BOARD_H_
#define BOARD_H_

/*  board.c contiene todas las declaraciones y funciones que crean o imprimen el tablero
    y posicionan los barcos para el inicio de la partida, ademas de varias funciones
    realcionadas al tablero adicionales.
*/

#ifndef FALSE
 #define FALSE 0
#endif //FALSE

#ifndef TRUE
 #define TRUE 1
#endif //TRUE

#define FILAS 10
#define COLUMNAS 10

#define HORIZONTAL 0
#define VERTICAL 1

//estas definiciones sirven para aclarar el codigo
#define PLAYER_1 0
#define PLAYER_2 1

#define JVP 0   //jugador vs programa
#define PVP 1   //programa vs programa

/*
    las siguientes definiciones sirven para imprimir los tableros y son los
    datos guardados en los arrays tableros
*/
#define FRAG 'F'
#define DEST 'D'
#define ACOR 'A'
#define SUBM 'S'
#define PORT 'P'

#define AGUA 0
#define HIT 'X'
#define MISS '~'
#define SUNK '*'

struct coord
{
    int x, y;
};

struct ship     //estructura que guarda informaciones sobre cada barco
{
    int size, len;  //size 0-4, len 1-4
    int orient;     //horizontal o vertical
    int live;       //0 a len segun las celdas que no fueron disparadas
    struct coord *pos;        //puntero a coordenadas de cada celda ocupada;
};

/* Imprime el tablero especificado para el jugador player. */
void print_board(int player, int tipo);
    /* Parametros:
        player: 0 o 1
        tipo (de tablero): 0 o 1, tablero de flota o enemigo
    */

/* Resetea todos los tableros a {0} */
void reset();

/* Permite configurar la cantidad de cada tipo de barco que tendran los jugadores.
   Guarda cantidades por tipo en fleetsize[] y total en fleetsize_n. */
void set_fleetsize(int n_fra, int n_des, int n_aco, int n_sub, int n_por, int n_tot);

/* Ubica un barco sobre el tablero y genera un struct ship */
int place_ship(int player, int ship_size, int ship_n, int orient, int x, int y);
    /* Parametros:
        player: 0 o 1. En modo PvE, 0 es el jugador, 1 es la computadora
        ship_size: 0 a 4
        ship_n: 0 a fleetsize[ship_size]
        orient: 0 a 1
        x,y: coordenadas, 0 a 9

    ret:
    !error
      return 0;
    error fuera del tablero
      return -1;
    error solapamiento de barcos
      return -2;
    return -3 si llega al final de la funcion, no deberia pasar
    */

/* Cuenta la cantidad de barcos vivos en la flota del jugador */
int count_ships(int player);

/* Cuenta la cantidad de barcos vivos del tipo type en la flota del jugador */
int count_ship_type(int player, int type);
/* type: 0-4 (fragata - portaviones) */

/* Marca barcos hundidos para distinguirlos de los barcos disparados pero vivos */
int mark_sunk(int player);
    /* ret:
        0-4: tipo de barco hundido
        -1: ningun barco hundido
    */

#endif // BOARD_H_
