#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "ui.h"
#include "gameloop.h"

int select_gamemode()
{
    /* Permite al usuario seleccionar el modo de juego */
    int s, aux;

    printf("\n<?>\tModo de juego");
    printf("\n    Seleccione el modo de juego.");
    printf("\n    1- Jugador vs Computadora");
    printf("\n    2- Programa vs Programa (no disponible)\n");
    do
    {
        printf("\t  > ");
        s = scanf("%d", &aux);
        while (getchar() != '\n');
        if (aux != 1 && aux != 2)
            s = 0;
    } while (s < 1);

    //aux--;
    //return aux;

    if (aux == 2) printf("\n<!>\tModo de juego no disponible\n    Modo \"1- Jugador vs Computadora\" seleccionado.\n\n");
    return 0;
}

int select_difficulty()
{
    /* Permite al usuario seleccionar la dificultad del oponente */
    int s, aux;

    printf("\n<?>\tDificultad");
    printf("\n    Seleccione la dificultad.");
    printf("\n    0- Inexistente");
    printf("\n    1- Normal");
    printf("\n    2- Dificil\n");
    do
    {
        printf("\t  > ");
        s = scanf("%d", &aux);
        while (getchar() != '\n');
        if (aux < 0 || aux > 2)
            s = 0;
    } while (s < 1);

    return aux;
}

void set_fleetsize_ui()
{
    int s;  //para comprobar resultados de scanf

    /* aux guarda valores temporalmente, sum cuenta la cantidad de barcos, valid es booleano auxiliar */
    int aux, sum = 0, valid;
    int fra = 0, des = 0, aco = 0, sub = 0, por = 0;    //cantidad de barcos de cada tipo

    printf("\n<?>\tComposicion de flotas");
    printf("\n    Ingrese la cantidad de cada tipo de barco para la partida.");
    printf("\n    Solo pueden haber 0-4 barcos de un tipo y maximo de 9 barcos en total.");

    printf("\n\nFragatas (1 celda):\n");
    do
    {
        valid = TRUE;
        printf("\t  > ");
        s = scanf("%d", &aux);
        while (getchar() != '\n');
        if (aux < 0 || aux > 4)
        {
            valid = FALSE;
            printf("<!>\tSolo pueden haber 0-4 barcos de un tipo!\n");
        }
    } while (s < 1 || valid == FALSE);

    fra = aux;
    sum += aux;


    printf("\n\nDestructores (2 celdas):\n");
    do
    {
        valid = TRUE;
        printf("\t  > ");
        s = scanf("%d", &aux);
        while (getchar() != '\n');
        if (aux < 0 || aux > 4)
        {
            valid = FALSE;
            printf("<!>\tSolo pueden haber 0-4 barcos de un tipo!\n");
        }
    } while (s < 1 || valid == FALSE);

    des = aux;
    sum += aux;


    printf("\n\nAcorazados (3 celdas):\n");
    do
    {
        valid = TRUE;
        printf("\t  > ");
        s = scanf("%d", &aux);
        while (getchar() != '\n');
        if (aux < 0 || aux > 4)
        {
            valid = FALSE;
            printf("<!>\tSolo pueden haber 0-4 barcos de un tipo!\n");
        }
        else        //a partir de este punto las sumas de los baros ya pueden sobrepasar el limite de 9
        {           //  por lo que hay que revisar que eso no ocurra
            valid = aux + sum;
            if (valid > 9)
                {
                    valid = FALSE;
                    printf("<!>\tSolo pueden haber 9 barcos en total!\n");
                }
            else
                valid = TRUE;
        }
    } while (s < 1 || valid == FALSE);

    aco = aux;
    sum += aux;


    printf("\n\nSubmarinos (3 celdas):\n");
    do
    {
        valid = TRUE;
        printf("\t  > ");
        s = scanf("%d", &aux);
        while (getchar() != '\n');
        if (aux < 0 || aux > 4)
        {
            valid = FALSE;
            printf("<!>\tSolo pueden haber 0-4 barcos de un tipo!\n");
        }
        else
        {
            valid = aux + sum;
            if (valid > 9)
                {
                    valid = FALSE;
                    printf("<!>\tSolo pueden haber 9 barcos en total!\n");
                }
            else
                valid = TRUE;
        }
    } while (s < 1 || valid == FALSE);

    sub = aux;
    sum += aux;


    printf("\n\nPortaviones (4 celdas):\n");
    do
    {
        valid = TRUE;
        printf("\t  > ");
        s = scanf("%d", &aux);
        while (getchar() != '\n');
        if (aux < 0 || aux > 4)
        {
            valid = FALSE;
            printf("<!>\tSolo pueden haber 0-4 barcos de un tipo!\n");
        }
        else
        {
            valid = aux + sum;
            if (valid > 9)
                {
                    valid = FALSE;
                    printf("<!>\tSolo pueden haber 9 barcos en total!\n");
                }
            else
                valid = TRUE;
        }
    } while (s < 1 || valid == FALSE);

    por = aux;
    sum += aux;

    if (sum == 0)
    {
        printf("\n<!>\tNo se agregaron barcos a la flota. Por favor reinicie.\n");
        set_fleetsize_ui();
        return;
    } else
    {
        printf("\n\n\tTotal de barcos: %d\n", sum);
        printf("\n<?>\tCambiar o confirmar configuracion:");
        printf("\n    1- Cambiar");
        printf("\n    2- Confirmar\n");

        do
        {
            printf("\t  > ");
            s = scanf("%d", &valid);
            while (getchar() != '\n');
            valid--;
        } while (s < 1 || (valid < 0 || valid > 1));

        while (valid == FALSE)      //el while no tiene tiene condicion de salida real, valid siempre queda falso
        {                           //  pero el usuario puede causar un break
            printf("\n\tQue quiere modificar?");
            printf("\n    1- Fragatas: %d", fra);
            printf("\n    2- Destructores: %d", des);
            printf("\n    3- Acorazados: %d", aco);
            printf("\n    4- Submarinos: %d", sub);
            printf("\n    5- Portaviones: %d", por);
            printf("\n    0- Confirmar\n");

            do
            {
                printf("\t  > ");
                s = scanf("%d", &aux);
                while (getchar() != '\n');
            } while (s < 1 || (aux < 0 || aux > 5));

            if (aux == 0)   //rompe el while
                break;

            switch (aux)
            {
            case 1:
                printf("\n\nFragatas (1 celda):\n");
                do
                {
                    valid = TRUE;
                    printf("\t  > ");
                    s = scanf("%d", &aux);
                    while (getchar() != '\n');
                    if (aux < 0 || aux > 4)
                    {
                        valid = FALSE;
                        printf("<!>\tSolo pueden haber 0-4 barcos de un tipo!\n");
                    }
                    else        //a diferencia de la misma funcionalidad al comienzo de la funcion
                    {           //  ahora el cambio de numero de este barco puede sobrepasar el limite de 9
                        valid = aux + sum;
                        if (fra > aux)     //si el limite de 9 ya se alcanzo, valid sera > 9
                            valid = TRUE;           //  pero si aux es menor al valor presente, sum sera < 9
                        else if (valid > 9)
                        {
                            valid = FALSE;
                            printf("<!>\tSolo pueden haber 9 barcos en total!\n");
                        }
                        else
                            valid = TRUE;
                    }
                } while (s < 1 && valid == FALSE);
                valid = FALSE;      //el do while solo rompe al ser valid == TRUE, pero el while mayor no debe romper

                sum -= fra;    //se resta el valor actual de la suma
                fra = aux;
                sum += aux;             //se adiciona el valor nuevo a la suma
                break;

            case 2:
                printf("\n\nDestructores (2 celdas):\n");
                do
                {
                    valid = TRUE;
                    printf("\t  > ");
                    s = scanf("%d", &aux);
                    while (getchar() != '\n');
                    if (aux < 0 || aux > 4)
                    {
                        valid = FALSE;
                        printf("<!>\tSolo pueden haber 0-4 barcos de un tipo!\n");
                    }
                    else
                    {
                        valid = aux + sum;
                        if (des > aux)
                            valid = TRUE;
                        else if (valid > 9)
                        {
                            valid = FALSE;
                            printf("<!>\tSolo pueden haber 9 barcos en total!\n");
                        }
                        else
                            valid = TRUE;
                    }
                } while (s < 1 && valid == FALSE);
                valid = FALSE;

                sum -= des;
                des = aux;
                sum += aux;
                break;

            case 3:
                printf("\n\nAcorazados (3 celdas):\n");
                do
                {
                    valid = TRUE;
                    printf("\t  > ");
                    s = scanf("%d", &aux);
                    while (getchar() != '\n');
                    if (aux < 0 || aux > 4)
                    {
                        valid = FALSE;
                        printf("<!>\tSolo pueden haber 0-4 barcos de un tipo!\n");
                    }
                    else
                    {
                        valid = aux + sum;
                        if (aco > aux)
                            valid = TRUE;
                        else if (valid > 9)
                        {
                            valid = FALSE;
                            printf("<!>\tSolo pueden haber 9 barcos en total!\n");
                        }
                        else
                            valid = TRUE;
                    }
                } while (s < 1 && valid == FALSE);
                valid = FALSE;

                sum -= aco;
                aco = aux;
                sum += aux;
                break;

            case 4:
                printf("\n\nSubmarinos (3 celdas):\n");
                do
                {
                    valid = TRUE;
                    printf("\t  > ");
                    s = scanf("%d", &aux);
                    while (getchar() != '\n');
                    if (aux < 0 || aux > 4)
                    {
                        valid = FALSE;
                        printf("<!>\tSolo pueden haber 0-4 barcos de un tipo!\n");
                    }
                    else
                    {
                        valid = aux + sum;
                        if (sub > aux)
                            valid = TRUE;
                        else if (valid > 9)
                        {
                            valid = FALSE;
                            printf("<!>\tSolo pueden haber 9 barcos en total!\n");
                        }
                        else
                            valid = TRUE;
                    }
                } while (s < 1 && valid == FALSE);
                valid = FALSE;

                sum -= sub;
                sub = aux;
                sum += aux;
                break;

            case 5:
                printf("\n\nPortaviones (4 celdas):\n");
                do
                {
                    valid = TRUE;
                    printf("\t  > ");
                    s = scanf("%d", &aux);
                    while (getchar() != '\n');
                    if (aux < 0 || aux > 4)
                    {
                        valid = FALSE;
                        printf("<!>\tSolo pueden haber 0-4 barcos de un tipo!\n");
                    }
                    else
                    {
                        valid = aux + sum;
                        if (por > aux)
                            valid = TRUE;
                        else if (valid > 9)
                        {
                            valid = FALSE;
                            printf("<!>\tSolo pueden haber 9 barcos en total!\n");
                        }
                        else
                            valid = TRUE;
                    }
                } while (s < 1 && valid == FALSE);
                valid = FALSE;

                sum -= por;
                por = aux;
                sum += aux;
                break;
            }
            printf("\n\n\tTotal de barcos: %d\n", sum);
        }
    }

    /* llama la funcion asociada de board.c */
    set_fleetsize(fra, des, aco, sub, por, sum);
}

void set_max_shots()
{
    /* Pregunta si el jugador desea poner un limite de disparos, al llegar al limite termina el juego */
    int s, aux;
    extern int limite_disparos;

    printf("\n<?>\tLimite de disparos");
    printf("\n    Ingrese, si desea, la cantidad de disparos permitidos por jugador.");
    printf("\n    Al acabarse los disparos, termina la partida.");
    printf("\n    1- Sin limite");
    printf("\n    2- Con limite\n");
    do
    {
        printf("\t  > ");
        s = scanf("%d", &aux);
        while (getchar() != '\n');
        if (aux != 1 && aux != 2)
            s = 0;
    } while (s < 1);

    if (aux == 1)
        limite_disparos = 100;
    else
    {
        printf("\n\tCual es el limite? (1-100)\n");
        do
        {
            printf("\t  > ");
            s = scanf("%d", &aux);
            while (getchar() != '\n');
            if (aux < 1 || aux > 100)
                s = 0;
        } while (s < 1);
        limite_disparos = aux;
    }
}

void set_turn()
{
    /* Pregunta al usuario cual jugador debe empezar a disparar */

    int s, aux;
    extern int turn;

    printf("\n<?>\tQuien Comienza disparando?");
    printf("\n    1- Jugador");
    printf("\n    2- Computadora\n");
    do
    {
        printf("\t  > ");
        s = scanf("%d", &aux);
        while (getchar() != '\n');
        if (aux != 1 && aux != 2)
            s = 0;
    } while (s < 1);
    turn = aux - 1;
}

void print_help()
{
    /* Imprime instrucciones de juego y aclara las reglas. */

    printf("\nInstrucciones:");
    delay(0.25);
    printf("\n  Configuracion inicial:");
    printf("\n    -Se elige un modo de juego.");
    printf("\n    -Se elige la configuracion inicial de las flotas.");
    printf("\n      (0-4 barcos de cada tipo, maximo 9 barcos)");
    printf("\n    -Se elige o no un limite de disparos.");
    printf("\n    -Se elige quien comienza jugando.\n");
    delay(0.25);
    printf("\n  Configuracion de flota:");
    printf("\n    -Los jugadores ubican las flotas sobre tableros de 10x10 celdas.");
    printf("\n      (se elige una fila, una columna y la orientacion del barco)");
    printf("\n      <!> Filas son horizontales, columnas son verticales!");
    printf("\n      <!> Solo se introducen coordenadas para la primera celda del barco!\n");
    delay(0.25);
    printf("\n  Inicio de partida:");
    printf("\n    -Se disparan posiciones enemigas introduciendo coordenadas.");
    printf("\n    -Si se acierta, el disparador vuelve a jugar.\n");
    delay(0.25);
    printf("\n  Fin de partida:");
    printf("\n    -Gana el jugador que hunda la flota enemiga.");
    printf("\n    -En caso de terminarse los disparos, gana el jugador con mas barcos vivos.\n");
    delay(0.5);

    printf("\n<?>\tPresione ENTER para empezar\n");
    while(getchar() != '\n');
}

void print_ref()
{
    /* Imprime referencias a simbolos que aparecen sobre el tablero conforme avance el juego */

    printf("\n Significados de celdas:");
    printf("\n  (Vacia): Celda no fue disparada");
    printf("\n  ~: Disparo fallido");
    printf("\n  X: Disparo certero");
    printf("\n  *: Barco hundido");
    printf("\n\n  F: Fragata");
    printf("\n  D: Destructor");
    printf("\n  A: Acorazado");
    printf("\n  S: Submarino");
    printf("\n  P: Portaviones");
}

void ui_settings()
{
    /* Junta todas las configuraciones iniciales e imprime el mensaje de bienvenida */

    extern int gamemode;
    extern int difficulty;

    printf("\n    -==< BIENVENIDO A BATALLA NAVAL >==-\n\n");

    delay(2);
    print_help();

    printf("\n\n  ## CONFIGURACION DE PARTIDA ##\n\n");
    gamemode = select_gamemode();
    difficulty = select_difficulty();
    set_fleetsize_ui();
    set_max_shots();
    set_turn();
}

void place_ship_ui(int player, int ship_size, int ship_n)
{
    int s, aux;
    int x, y, orient;
    int error;

    switch (ship_size)
    {
    case 0:
        printf("\nFragata (1 celda):");
        break;
    case 1:
        printf("\nDestructor (2 celdas):");
        break;
    case 2:
        printf("\nAcorazado (3 celdas):");
        break;
    case 3:
        printf("\nSubmarino (3 celdas):");
        break;
    case 4:
        printf("\nPortaviones (4 celdas):");
        break;
    }

    printf("\n\nFila (1-10):\n");
    do
    {
        printf("\t  > ");
        s = scanf("%d", &aux);
        while (getchar() != '\n');
        if (aux < 1 || aux > 10)
            s = 0;
    } while (s < 1);
    x = aux;

    printf("\n\nColumna (1-10):\n");
    do
    {
        printf("\t  > ");
        s = scanf("%d", &aux);
        while (getchar() != '\n');
        if (aux < 1 || aux > 10)
            s = 0;
    } while (s < 1);
    y = aux;

    if (ship_size > 0)
    {
        printf("\n\tOrientacion:");
        printf("\n    1- Horizontal");
        printf("\n    2- Vertical\n");
        do
        {
            printf("\t  > ");
            s = scanf("%d", &aux);
            while (getchar() != '\n');
            if (aux != 1 && aux != 2)
                s = 0;
        } while (s < 1);
        aux--;
        orient = aux;
    } else orient = 0;

    error = place_ship(player, ship_size, ship_n, orient, x-1, y-1);

    if (error == -1)
    {
        printf("\n<!>\tYa hay un barco en esa zona!\n");
        place_ship_ui(player, ship_size, ship_n);           //recursion
    } else if (error == -2)
    {
        printf("\n<!>\tEl barco sale del tablero!\n");
        place_ship_ui(player, ship_size, ship_n);           //recursion
    }
}

void setup_board()
{
    /* Coordina llamadas de place_ship_ui() */
    extern int fleetsize[5];

    printf("\n<?>\tUbique sus barcos");
    printf("\n    Ingrese las coordenadas de la primera celda ocupada por el barco.");
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < fleetsize[i]; j++)
        {
            print_board(PLAYER_1, 0);
            place_ship_ui(PLAYER_1, i, j);
        }
    }
    print_board(PLAYER_1, 0);
}

int shoot_ui()
{
    /* Llama a validateShot() y shoot() con interfaz para el jugador. */

    int s, aux, res = 0;
    int fila, col;
    extern int turn;

    print_board(PLAYER_1, 1);  //imprime el tablero de disparos

    printf("\n<?>\tCoordenadas a disparar:\n");

    printf("\n\nFila (1-10):\n");
    do
    {
        printf("\t  > ");
        s = scanf("%d", &aux);
        while (getchar() != '\n');
        if (aux < 1 || aux > 10)
            s = 0;
    } while (s < 1);
    fila = aux-1;

    printf("\n\nColumna (1-10):\n");
    do
    {
        printf("\t  > ");
        s = scanf("%d", &aux);
        while (getchar() != '\n');
        if (aux < 1 || aux > 10)
            s = 0;
    } while (s < 1);
    col = aux-1;

    res = validateShot(PLAYER_1, fila, col);
    if (res == 0)
    {
        printf("\nFallo!\n\n");
        shoot(PLAYER_1, fila, col);
    }
    else if (res == 1)
    {
        printf("\nAcierto!\n Dispara otra vez\n\n");
        shoot(PLAYER_1, fila, col);
    }
    else if (res == -1)
    {
        printf("\n  Fuera del tablero!\n");
        delay(1);
        res = shoot_ui();
    }
    else if (res == -2)
    {
        printf("\n  Celda ya disparada!\n");
        delay(1);
        res = shoot_ui();
    }

    return res;
}

void game_over_msg(int winner)
{
    /* Imprime un mensaje de fin de juego y los resutados */

    extern int fleetsize_n;
    extern int lives_1;
    extern int lives_2;

    printf("\n\n  ## FIN DEL JUEGO ##\n");
    delay(1);
    if (winner == PLAYER_1)
    {
        printf("\n    Usted gana!\n");
    } else if (winner == PLAYER_2)
    {
        printf("\n    La computadora gana!\n");
    } else if (winner == -1)
    {
        printf("\n    Empate!\n");
    }

    printf("\n\n  Barcos hundidos:");
    printf("\n\tJugador: %d", (fleetsize_n - lives_2));
    printf("\n\tComputadora: %d\n", (fleetsize_n - lives_1));
}

void brief(int disp, int disp_ai)
{
    /* Imprime un reporte para el usuario. Incluye informaciones sobre las dos flotas y cantidad de disparos */

    extern int fleetsize_n;
    extern int fleetsize[5];
    int nF, nD, nA, nS, nP;
    extern int limite_disparos;

    printf("\n Estado de la batalla:");
    print_board(PLAYER_1, 0);
    delay(1);
    printf("\n  %d/%d Barcos aliados totales", count_ships(PLAYER_1), fleetsize_n);
        //barcos aliados por tipo, si existen
    if (fleetsize[0] > 0)
    {
        for (int i = 0; i < fleetsize[0]; i++)
        {
            nF = count_ship_type(PLAYER_1, 0);
        }
        printf("\n   %d/%d Fragatas", nF, fleetsize[0]);
    }
    if (fleetsize[1] > 0)
    {
        for (int i = 0; i < fleetsize[1]; i++)
        {
            nD = count_ship_type(PLAYER_1, 1);
        }
        printf("\n   %d/%d Destructores", nD, fleetsize[1]);
    }
    if (fleetsize[2] > 0)
    {
        for (int i = 0; i < fleetsize[2]; i++)
        {
            nA = count_ship_type(PLAYER_1, 2);
        }
        printf("\n   %d/%d Acorazados", nA, fleetsize[2]);
    }
    if (fleetsize[3] > 0)
    {
        for (int i = 0; i < fleetsize[3]; i++)
        {
            nS = count_ship_type(PLAYER_1, 3);
        }
        printf("\n   %d/%d Submarinos", nS, fleetsize[3]);
    }
    if (fleetsize[4] > 0)
    {
        for (int i = 0; i < fleetsize[4]; i++)
        {
            nP = count_ship_type(PLAYER_1, 4);
        }
        printf("\n   %d/%d Portaviones\n", nP, fleetsize[4]);
    }

    printf("\n  %d/%d Barcos enemigos totales", count_ships(PLAYER_2), fleetsize_n);
        //barcos enemigos por tipo, si existen
    if (fleetsize[0] > 0)
    {
        for (int i = 0; i < fleetsize[0]; i++)
        {
            nF = count_ship_type(PLAYER_2, 0);
        }
        printf("\n   %d/%d Fragatas", nF, fleetsize[0]);
    }
    if (fleetsize[1] > 0)
    {
        for (int i = 0; i < fleetsize[1]; i++)
        {
            nD = count_ship_type(PLAYER_2, 1);
        }
        printf("\n   %d/%d Destructores", nD, fleetsize[1]);
    }
    if (fleetsize[2] > 0)
    {
        for (int i = 0; i < fleetsize[2]; i++)
        {
            nA = count_ship_type(PLAYER_2, 2);
        }
        printf("\n   %d/%d Acorazados", nA, fleetsize[2]);
    }
    if (fleetsize[3] > 0)
    {
        for (int i = 0; i < fleetsize[3]; i++)
        {
            nS = count_ship_type(PLAYER_2, 3);
        }
        printf("\n   %d/%d Submarinos", nS, fleetsize[3]);
    }
    if (fleetsize[4] > 0)
    {
        for (int i = 0; i < fleetsize[4]; i++)
        {
            nP = count_ship_type(PLAYER_2, 4);
        }
        printf("\n   %d/%d Portaviones\n", nP, fleetsize[4]);
    }

    if (limite_disparos == 100) return; //caso no hay limite
    else if (limite_disparos - disp > 0)
    {
        printf("\n  %d Disparos restantes\n", limite_disparos - disp);
    } else
    {
        printf("\n  %d Disparos (enemigos) restantes", limite_disparos - disp_ai);
        printf("\n  (Cuando ambos lleguen al limite, el juego termina)\n");
    }

    delay(1.5);
}

void ai_shot_msg(int x, int y, int res)
{
    /* Prints a message to stdout informing the user of PLAYER_2's turn */
    printf("\n<!>\tOponente dispara en (%d, %d)!\n", x+1, y+1);
    if (res == 0) printf("\nFallo!\n\n");
    else if (res == 1) printf("\nAcierto!\n Dispara otra vez\n\n");
}
