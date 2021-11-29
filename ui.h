#ifndef UI_H_
#define UI_H_

/* Para la version de consola.
    Todas las funciones imprimen textos y preguntas y toman entradas de datos relacionadas a las
    funciones paralelas de nombre similar en board.c. Actuan de interfaz de usuario.
*/

/* Permite al usuario seleccionar el modo de juego */
int select_gamemode();
    /* retorna 0 o 1 */

/* Permite al jugador insertar la cantidad de botes de cada tipo usada en la partida */
void set_fleetsize_ui();

/* Pregunta si el jugador desea poner un limite de disparos, al llegar al limite termina el juego */
void set_max_shots();

/* Pregunta al usuario cual jugador debe empezar a disparar */
void set_turn();

/* Junta todas las configuraciones iniciales e imprime el mensaje de bienvenida */
void ui_settings();
    /* Llamadas:
        select_gamemode();
        set_fleetsize_ui();
        set_max_shots();
        set_turn();
    */

/* Lee coordenadas y orientacion del jugador */
void place_ship_ui(int player, int ship_size, int ship_n);
    /* Parametros:
        player: 0 o 1
        ship_size: 0 a 4
        ship_n: 0 a fleetsize[ship_size] -1, cuenta la cantidad de barcos de un tipo
            util en el almacenamiento de datos de cada barco
    */

/* Coordina llamadas de place_ship_ui() */
void setup_board();
    /* Hasta ahora solo llama la funcion con interfaz para los jugadores,
        pero en el futuro debe llamar la funcion sin interfaz para el programa.
    */

/* Llama a validateShot() y shoot() con interfaz para el jugador */
int shoot_ui();
    /* ret:
        0: fallo
        1: acierto
    */

/* Imprime un mensaje de fin de juego y los resutados */
void game_over_msg(int winner);

/* Imprime un reporte para el usuario. Incluye informaciones sobre las dos flotas */
void brief();

/* Prints a message to stdout informing the user of PLAYER_2's actions */
void ai_shot_msg(int x, int y, int res);
    /* Parametros:
        x, y: 0-9
        res: resultado del disparo: 0 fallo, 1 acierto
    */

#endif // UI_H_
