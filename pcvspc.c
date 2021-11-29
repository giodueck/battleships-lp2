#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include "board.h"
#include "gameloop.h"
#include "gtk_callbacks.h"
#include "prob_density.h"

char path[100];
extern gchar *p_name;
extern gchar *c_name;

extern int fase;
extern int turn;
extern int fleetsize[5];
extern int fleetsize_n;

extern int lives_1;
extern int lives_2;

extern int fleetb_1[10][10];
extern struct PDcell PDboard[10][10];
extern struct ship *fleet_1;

int mystery_board[10][10] = {0};

extern int tot_p1_shots;
extern int tot_p1_hits;
extern int tot_p2_shots;
extern int tot_p2_hits;
int total_shots = 0;
int total_cells = 0;

void pvp_set_path(gchar *folder)
{
    /* Guarda el path al directorio en el que se juega */

    //file_chooser guarda la cadena iniciando con "file:///", solo ne necesita los caracteres siguiendo esta subcadena
    strcpy(path, &folder[8]);
    strcat(path, "/");
    puts(path);
}

void pvp_setup_board()
{
    /* Coloca los barcos del programa
        Codigo adaptado de la funcion setup_board_PD de prob_density
    */

    int len;
    int error;
    int x, y, orient;
    int first = 1;

    for (int size = 0; size < 5; size++)
    {
        if (size < 3)
            len = size + 1;
        else
            len = size;

        for (int n = 0; n < fleetsize[size]; n++)
        {
            total_cells += len;

            do
            {
                x = rand() % 10;
                y = rand() % 10;
                orient = rand() % 2;

                error = 0;

                if (first)  //first ship will be positioned on an edge
                {
                    if (x == 0 || x == 9 || y == 0 || y == 9)
                    {
                        if (place_ship(PLAYER_1, size, n, orient, x, y) == 0)
                        {
                            first = 0;
                            error = 0;
                            continue;
                        }
                        else
                        {
                            error = 1;
                            continue;
                        }
                    } else
                    {
                        error = 1;
                        continue;
                    }
                } else  //ships will be placed not touching each other
                {
                    if (orient == HORIZONTAL)
                    {
                        for (int j = 0; j < len; j++)   //tests all cells along the length of the ship
                        {
                            if (x+1 <= 9 && fleetb_1[x+1][y+j] != 0) //must not touch a ship to either side
                            {
                                error = 1;
                                break;
                            } else if (x-1 >= 0 && fleetb_1[x-1][y+j] != 0)
                            {
                                error = 1;
                                break;
                            } else if (y-1 >= 0 && fleetb_1[x][y-1] != 0)   //must not touch either end with a ship
                            {
                                error = 1;
                                break;
                            } else if (y+len <= 9 && fleetb_1[x][y+len] != 0)
                            {
                                error = 1;
                                break;
                            }
                        }
                    } else if (orient == VERTICAL)
                    {
                        for (int i = 0; i < len; i++)
                        {
                            if (y+1 <= 9 && fleetb_1[x+i][y+1] != 0) //must not touch a ship to either side
                            {
                                error = 1;
                                break;
                            } else if (y-1 >= 0 && fleetb_1[x+i][y-1] != 0)
                            {
                                error = 1;
                                break;
                            } else if (x-1 >= 0 && fleetb_1[x-1][y] != 0)   //must not touch either end with a ship
                            {
                                error = 1;
                                break;
                            } else if (x+len <= 9 && fleetb_1[x+len][y] != 0)
                            {
                                error = 1;
                                break;
                            }
                        }
                    }
                }

                if (error == 0)
                    error = place_ship(PLAYER_1, size, n, orient, x, y);
            } while (error);
        }
    }
}

void reset_mystery()
{
    /* Reinicializa mystery_bpard */
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            mystery_board[i][j] = 0;
        }
    }
}

void mark_mystery(int x, int y, int res)
{
    /* Marca la posicion indicada en el tablero desconocido del oponente
        Parametros:
            x: fila
            y: columna
            res: resultado del disparo: 0 agua, 1 acierto
    */

    if (res == 0)
        mystery_board[x][y] = MISS;
    else if (res == 1)
        mystery_board[x][y] = HIT;
    else
        mystery_board[x][y] = SUNK;
}

void pvp_shoot(int *x, int *y)
{
    /* Realiza un disparo, crea un archivo de disparo.
    */

    FILE * f;
    char filename[100];

    //creacion del filename
    strcpy(filename, path);
    strcat(filename, p_name);
    strcat(filename, ".txt");

    //disparo
        //codigo adaptado de prob_density
    int max_density;
    struct PDcell possibilities[POSSIBILITY_AMNT];    //cells with the highest probability will be stored for one to be shot
    int ctr = 0;
    int shoot_at;   //takes a value between 0 and ctr-1

    calculate_density();

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (i == 0 && j == 0)                           //first maximum
                max_density = PDboard[i][j].density;
            else if (PDboard[i][j].density > max_density)    //update max_density
                max_density = PDboard[i][j].density;
        }
    }

    for (int i = 0; i < 10 && ctr < POSSIBILITY_AMNT-1; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (ctr == POSSIBILITY_AMNT-1)   //only 20 cells will be stored
            {
                break;
            } else if (PDboard[i][j].density == max_density)   //store cell
            {
                possibilities[ctr] = PDboard[i][j];
                ctr++;
            }
        }
    }

    //generate number between 0 and ctr-1 and shoot at that cell
    shoot_at = rand() % ctr;
    *x = possibilities[shoot_at].x;
    *y = possibilities[shoot_at].y;

        //necesario para el correcto funcionamiento del PDA, poner en pvp_get_response
//    if (hit)
//        PDboard[x][y].state = HIT;
//    else
//        PDboard[x][y].state = MISS;

    //creacion del archivo
    f = fopen(filename, "wt");

    fprintf(f, "Fila: %d\n", *x);
    fprintf(f, "Columna: %d", *y);

    fclose(f);
}

int pvp_get_response(int *x, int *y)
{
    /* Lee el archivo respuesta a un disparo y registra los resultados sobre el tablero propio
        Retorno:
            -1: ninguna respuesta
            -2: error de formato
            0: fallo
            1: acierto
            2: hundimiento
    */

    FILE * f;
    int res = 0;
    int sunk;
    char filename[100];
    char str[24];
    char *pos[4]; //ej. "(0,3)\0"

    //creacion del filename
    strcpy(filename, path);
    strcat(filename, "respuesta");
    strcat(filename, p_name);
    strcat(filename, ".txt");

    //lectura
    f = fopen(filename, "rb");
    fgets(str, 24, f);  //resultado

    if (f == NULL)
    {
        return -1;
    }

    if ((strcmp(str, "agua")) == 0 || (strcmp(str, "agua\n")) == 0 || (strcmp(str, "agua\r\n")) == 0)
    {
        res = 0;
        PDboard[*x][*y].state = MISS;
        mark_mystery(*x, *y, res);
    } else if ((strcmp(str, "acierto")) == 0 || (strcmp(str, "acierto\n")) == 0 || (strcmp(str, "acierto\r\n")) == 0)
    {
        res = 1;
        tot_p1_hits++;
        PDboard[*x][*y].state = HIT;
        mark_mystery(*x, *y, res);
    } else if ((strcmp(str, "hundimiento")) == 0 || (strcmp(str, "hundimiento\n")) == 0 || (strcmp(str, "hundimiento\r\n")) == 0)
    {
        res = 2;
        tot_p1_hits++;
        lives_2--;

        fgets(str, 24, f);  //tipo de barco

        if (strcmp(str, "FRAGATA") == 0 || strcmp(str, "FRAGATA\n") == 0 || strcmp(str, "FRAGATA\r\n") == 0)
        {
            sunk = 0;
            fgets(str, 24, f);  //posicion
            pos[0] = strtok(str, " ");

            *x = pos[0][1] - '0';
            *y = pos[0][3] - '0';
            PDboard[*x][*y].state = SUNK;
            mark_mystery(*x, *y, res);

        } else if (strcmp(str, "DESTRUCTOR") == 0 || strcmp(str, "DESTRUCTOR\n") == 0 || strcmp(str, "DESTRUCTOR\r\n") == 0)
        {
            sunk = 1;
            fgets(str, 24, f);
            pos[0] = strtok(str, " ");

            for (int i = 1; i < 2; i++)
            {
                pos[i] = strtok(NULL, " \n");
            }

            for (int i = 0; i < 2; i++)
            {
                *x = pos[i][1] - '0';
                *y = pos[i][3] - '0';
                PDboard[*x][*y].state = SUNK;
                mark_mystery(*x, *y, res);
            }
        } else if (strcmp(str, "ACORAZADO") == 0 || strcmp(str, "ACORAZADO\n") == 0 || strcmp(str, "ACORAZADO\r\n") == 0)
        {
            sunk = 2;
            fgets(str, 24, f);
            pos[0] = strtok(str, " ");

            for (int i = 1; i < 3; i++)
            {
                pos[i] = strtok(NULL, " \n");
            }

            for (int i = 0; i < 3; i++)
            {
                *x = pos[i][1] - '0';
                *y = pos[i][3] - '0';
                PDboard[*x][*y].state = SUNK;
                mark_mystery(*x, *y, res);
            }
        } else if (strcmp(str, "SUBMARINO") == 0 || strcmp(str, "SUBMARINO\n") == 0 || strcmp(str, "SUBMARINO\r\n") == 0)
        {
            sunk = 3;
            fgets(str, 24, f);
            pos[0] = strtok(str, " ");

            for (int i = 1; i < 3; i++)
            {
                pos[i] = strtok(NULL, " \n");
            }

            for (int i = 0; i < 3; i++)
            {
                *x = pos[i][1] - '0';
                *y = pos[i][3] - '0';
                PDboard[*x][*y].state = SUNK;
                mark_mystery(*x, *y, res);
            }
        } else if (strcmp(str, "PORTAAVION") == 0 || strcmp(str, "PORTAAVION\n") == 0 || strcmp(str, "PORTAAVION\r\n") == 0)
        {
            sunk = 4;
            fgets(str, 24, f);
            pos[0] = strtok(str, " ");

            for (int i = 1; i < 4; i++)
            {
                pos[i] = strtok(NULL, " \n");
            }

            for (int i = 0; i < 4; i++)
            {
                *x = pos[i][1] - '0';
                *y = pos[i][3] - '0';
                PDboard[*x][*y].state = SUNK;
                mark_mystery(*x, *y, res);
            }
        } else
        {
            printf("Error: pvp_get_response(): formato no reconocido.\n");
            return -2;
        }

        mark_sunk_PD(sunk);
    }

    fclose(f);
    remove(filename);
    return res;
}

int pvp_respond(int *x, int *y)
{
    /* Realiza el diparo del oponente sobre los tableros propios. Escribe los resultados a un archivo de respuesta.
        Retorno:
            -1: ningun disparo
            0: fallo
            1: acierto
            2: hundimiento
    */

    FILE * f;

    int res;
    int size, len;

    char filename[100];
    char str[12];
    char *tok;

    struct coord cells[4];  //guarda las celdas del barco hundido

    //creacion del filename disparo del oponente
    strcpy(filename, path);
    strcat(filename, c_name);
    strcat(filename, ".txt");

    //lectura
    f = fopen(filename, "rt");

    if (f == NULL)
        return -1;

        //fila
    fgets(str, 12, f);
    strtok(str, " ");
    tok = strtok(NULL, " \n");
    *x = atoi(tok);
        //columna
    fgets(str, 12, f);
    strtok(str, " ");
    tok = strtok(NULL, " \n");
    *y = atoi(tok);

    fclose(f);
    remove(filename);

    //disparo
    res = validateShot(PLAYER_2, *x, *y);
    shoot(PLAYER_2, *x, *y);

    //revision de hundimiento
    if (res == 1)
    {
        tot_p2_hits++;
        for (int i = 0; i < fleetsize_n; i++)
        {
            if (fleet_1[i].live == FALSE)
            {
                if (fleetb_1[fleet_1[i].pos[0].x][fleet_1[i].pos[0].y] == HIT)
                {
                    res = 2;
                    lives_1--;

                    size = fleet_1[i].size;
                    if (size < 3)
                        len = size + 1;
                    else
                        len = size;

                    for (int j = 0; j < fleet_1[i].len; j++)
                    {
                        fleetb_1[fleet_1[i].pos[j].x][fleet_1[i].pos[j].y] = SUNK;
                        cells[j] = fleet_1[i].pos[j];
                    }
                }
            }
        }
    }

    //creacion del filename respuesta
    strcpy(filename, path);
    strcat(filename, "respuesta");
    strcat(filename, c_name);
    strcat(filename, ".txt");

    //escritura
    f = fopen(filename, "wt");

        //resultado
    if (res == 0)
        fputs("agua", f);
    else if (res == 1)
        fputs("acierto", f);
    else if (res == 2)
    {
        fputs("hundimiento\n", f);

        switch (size)
        {
        case 0:
            fputs("FRAGATA\n", f);
            break;
        case 1:
            fputs("DESTRUCTOR\n", f);
            break;
        case 2:
            fputs("ACORAZADO\n", f);
            break;
        case 3:
            fputs("SUBMARINO\n", f);
            break;
        case 4:
            fputs("PORTAAVION\n", f);
        }

        for (int i = 0; i < len; i++)
        {
            fprintf(f, "(%d,%d)", cells[i].x, cells[i].y);
            if (i != len-1) //espacio separador para todos menos el ultimo elemento
                fputc(' ', f);
        }
    }

    fclose(f);
    return res;
}

void pvp_end_game()
{
    /* Crea el archivo p_name.txt con la linea FIN */

    FILE * f;
    char filename[100];

    //creacion del filename
    strcpy(filename, path);
    strcat(filename, p_name);
    strcat(filename, ".txt");

    f = fopen(filename, "wt");
    fputs("FIN", f);
    fclose(f);

    run_game_over();
}

void pvp_reset_historial()
{
    /* Borra el archivo historial.txt y crea uno nuevo */

    FILE * h;
    char filename[100];

    //creacion del filename
    strcpy(filename, path);
    strcat(filename, "historial.txt");

    h = fopen(filename, "wt");
    fclose(h);
}

void pvp_clean()
{
    /* Elimina archivos con los nombres usados si es que existen */

    FILE * f;
    char filename[100];

    //jugador
    strcpy(filename, path);
    strcat(filename, p_name);
    strcat(filename, ".txt");

    if ((f = fopen(filename, "rt")) != NULL)
    {
        fclose(f);
        remove(filename);
    }

    strcpy(filename, path);
    strcat(filename, "respuesta");
    strcat(filename, c_name);
    strcat(filename, ".txt");

    if ((f = fopen(filename, "rt")) != NULL)
    {
        fclose(f);
        remove(filename);
    }

    //oponente
    strcpy(filename, path);
    strcat(filename, c_name);
    strcat(filename, ".txt");

    if ((f = fopen(filename, "rt")) != NULL)
    {
        fclose(f);
        remove(filename);
    }

    strcpy(filename, path);
    strcat(filename, "respuesta");
    strcat(filename, p_name);
    strcat(filename, ".txt");

    if ((f = fopen(filename, "rt")) != NULL)
    {
        fclose(f);
        remove(filename);
    }
}

void pvp_log_turn()
{
    /* Registra el turno en el historial */

    //Formato: <numero_de_jugada>;<nombre_jugador>;<cant_aciertos>;<cant_restante>

    FILE * f;
    char filename[100];
    char str[100];
    int left = total_cells - tot_p1_hits;

    //creacion del filename
    strcpy(filename, path);
    strcat(filename, "historial.txt");

    f = fopen(filename, "at");

    snprintf(str, 100, "%d;%s;%d;%d\n", total_shots, p_name, tot_p1_hits, left);
    fputs(str, f);

    fclose(f);
}

//int chk_historial()
//{
//    /* Revisa si la ultima entrada al historial esta correctamente formateada
//        Retorno:
//            0: error
//            1: correcto
//    */
//
//    //Formato: <numero_de_jugada>;<nombre_jugador>;<cant_aciertos>;<cant_restante>
//
//    FILE * f;
//    char filename[100];
//    char str[100];
//    char *substr[4];
//
//    //creacion del filename
//    strcpy(filename, path);
//    strcat(filename, "historial.txt");
//
//    f = fopen(filename, "rt");
//    fseek(f, 0, SEEK_SET);
//
//    for (int i = 0; i < total_shots; i++)
//    {
//        fgets(str, 100, f);
//    }
//
//    substr[0] = strtok(str, ";");
//    for (int i = 1; i < 4; i++)
//    {
//        substr[i] = strtok(NULL, ";\n");
//    }
//
//    if (atoi(substr[0]) == 0)
//    {
//        fclose(f);
//        return 0;
//    }
//    if (strcmp(substr[1], c_name) != 0)
//    {
//        fclose(f);
//        return 0;
//    }
//    if (atoi(substr[2]) != tot_p2_hits)
//    {
//        fclose(f);
//        return 0;
//    }
//    if (atoi(substr[3]) != total_cells-tot_p2_hits)
//    {
//        fclose(f);
//        return 0;
//    }
//
//    fclose(f);
//    return 1;
//}

int pvp_turn()
{
    /* Llama funciones segun turn; genera disparos o respuestas.
        Retorna TRUE o FALSE segun las condiciones de victoria. TRUE indica a g_timeout que el juego continua
    */
    static int res = 0;
    static int x = 0, y = 0;

    extern int gamemode;

    if (gamemode == 0)
        return FALSE;


    if (turn == PLAYER_1)   //este programa; PLAYER_2 sera el oponente
    {
        //eliminar archivos preexistentes
        if (total_shots == 0)
            pvp_clean();

        //disparo
        if (res >= 0)
        {
//            if (total_shots > 0 && chk_historial() == 0)
//                change_status(10);

            pvp_shoot(&x, &y);
            tot_p1_shots++;
            total_shots++;
        }

        printf("%d: Shoots (%d,%d)\n", tot_p1_shots, x, y);

        //lectura de respuesta
        res = pvp_get_response(&x, &y);
        if (res >= 0)
        {
            printf("%s responds %d\n", c_name, res);
            pvp_log_turn();
            update_board_images();
            update_ships_images();
        } else
            return TRUE;

        //fin del juego
        if (lives_2 == 0)
        {
            pvp_end_game();
            return FALSE;
        }

        //cambio de turno
        if (res == 0)
        {
            turn = PLAYER_2;
            change_status(9);
        }
        //else turn stays the same
    } else
    {
        //respuesta a disparo
        res = pvp_respond(&x, &y);
        if (res >= 0)
        {
            tot_p2_shots++;
            total_shots++;
            printf("%d: %s shoots (%d,%d)\n", tot_p1_shots, c_name, x, y);
            printf("Responds %d\n", res);
            update_board_images();
            update_ships_images();
        } else
            return TRUE;

        //fin del juego
        if (lives_1 == 0)
        {
            pvp_end_game();
            return FALSE;
        }

        //cambio de turno
        if (res == 0)
        {
            turn = PLAYER_1;
            change_status(9);
        }
    }

    return TRUE;
}
