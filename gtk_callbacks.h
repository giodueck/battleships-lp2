#ifndef GTK_CALLBACKS_H_
#define GTK_CALLBACKS_H_

/*  gtk_callbacks contiene todas las funciones callback del juego, es decir,
    llama funciones que modifican el estaado del juego. Callbacks para generar
    o modificar la interfaz se encuentran en el main.c
*/

// Struct para guardar estadisticas de juego para un jugador
struct player_stats
{
    char name[16];  //nombres son de 15 char como maximo
    unsigned int played;    //cantidades de partidas: total
    unsigned int won;       //ganadas
    unsigned int lost;      //perdidas
    unsigned int tied;      //empatadas
};

// Struct para guardar labels de una fila para stats_dialog
struct stats_row
{
    GtkLabel *name;
    GtkLabel *played;
    GtkLabel *won;
    GtkLabel *lost;
    GtkLabel *tied;
    GtkLabel *win_ratio;    //no parte de los requisitos
};

        //Funciones de confirmacion de salida

/* Si el juego esta en progreso, pregunta por una confirmacion antes de salir del programa.
   Se llama al usar la opcion de menu "Salir".
    Parametros:
        -Estandar CB
*/
void confirm_quit(GtkWidget *widget, gpointer data);

/* Si el juego esta en progreso, pregunta por una confirmacion antes de salir del programa.
   Se llama al crear el evento "delete-event" en window.
    Parametros:
        -Estandar CB
*/
gboolean confirm_quit_delete(GtkWidget *widget, GdkEvent  *event, gpointer data);


        //Funciones de configuracion

/* Funcion callback para mostrar el dialogo de configuracion de partida */
void run_config();

/* Carga la configuracion inicial y modifica los labels */
void init_config();

/* Cambia el gamemode seleccionado en el dialogo de configuracion
    Parametros:
        -Estandar CB
*/
void gamemode_cb(GtkWidget *widget, gpointer data);

/* Callback para diff_sel en en config
    Parametros:
        -Estandar CB
*/
void difficulty_cb(GtkWidget *widget, gpointer data);

/* Aumenta el contador del barco especificado en el dialogo de configuracion
    Parametros:
        -Estandar CB
            -data: FRAG, DEST, ACOR, SUBM o PORT
*/
void ship_add(GtkWidget *widget, gpointer data);

/* Decrementa el contador del barco especificado en el dialogo de configuracion
    Parametros:
        -Estandar CB
            -data: FRAG, DEST, ACOR, SUBM o PORT
*/
void ship_sub(GtkWidget *widget, gpointer data);

/* Callback para shot_entry
    Parametros:
        -Estandar CB
*/
void shot_cb(GtkWidget *widget, gpointer data);

/* Cambia el jugador que comienza el juego
    Parametros:
        -Estandar CB
*/
void start_cb(GtkWidget *widget, gpointer data);

/* Callback para el boton OK en config_dialog. Caso la configuracion sea valida,
   se aplica y se cierra el dialogo.
    Parametros:
        -Estandar CB
*/
void config_ok_cb(GtkWidget *widget, gpointer data);

/* Callback para el boton config_reset */
void config_reset_cb();


        //Funciones de path

/* Muestra el dialogo path_dialog */
void run_path();

/* Callback para path_ok. Verifica que un directorio se selecciona */
void path_ok_cb();


        //Funciones de instrucciones

/*  Funcion callback para mostrar el dialogo de instrucciones
    Parametros:
        -Estandar CB
*/
void run_instruc(GtkWidget *widget, gpointer data);

/* Cierra el cuadro de dialogo de instrucciones
    Parametros:
        -Estandar CB
*/
void instruc_ok_cb(GtkWidget *widget, gpointer data);


        //Funciones de game_over

/* Callback para game_over_new. Reinicia el juego */
void game_over_new_cb();

/* Callback para game_over_quit. Cierra el programa */
void game_over_quit_cb();


        //Funciones de about
/* Muestra el dialogo about */
void run_about();


        //Funciones de stats

/* Muestra el dialogo stats */
void run_stats();

/* Cierra el dialogo stats
    Parametros:
        -Estandar CB
*/
void stats_close_cb(GtkWidget *widget, gpointer data);


        //Funciones de tableros

/* Asigna imagenes de acuerdo a fleet_1 y fleet_2 al grid de la interfaz grafica */
void set_ships_images();

/* Reasigna imagenes de acuerdo a fleet_1 y fleet_2 al grid de la interfaz grafica */
void update_ships_images();

/* Reasigna imagenes de acuerdo al tablero interno al tablero de la interfaz grafica */
void update_board_images();

/* Cambia la orientacion seleccionada */
void orient_button_cb();

/* Confirma la seleccion de posicion y la guarda en la matriz interna */
void place_ship_ok_cb();

/* Dispara como PLAYER_2
    Retorno:
        TRUE: para que el timout siga corriendo
*/
int shoot_cpu_gui();

/* Funcion de callback para un evento button-press-event en un tablero
    Parametros:
        -Estandar CB
*/
void board_cb(GtkWidget *event_box, GdkEventButton *event, gpointer data);

/*  Crea el tablero del jugador usando un eventbox e imagenes de fondo.
      Retorno:
        el tablero
*/
GtkWidget * create_board_ju();

/*  Crea el tablero del oponente usando un eventbox e imagenes de fondo.
      Retorno:
        - el tablero
*/
GtkWidget * create_board_op();


        //Funciones de juego

/* Inicializa los tableros y la configuracion del juego como para iniciar un juego nuevo */
void new_game();

/* Si el juego esta en progreso, pregunta por una confirmacion antes de reiniciar el juego.
   Se llama al usar la opcion de menu "Nuevo".
    Parametros:
        -Estandar CB
*/
void confirm_restart(GtkWidget *widget, gpointer data);

/* El juego progresa a la fase de disparos. Desbloquea funciones de disparos y bloquea funciones
   de configuracion.
*/
void start_game();

#endif // GTK_CALLBACKS_H_
