#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "board.h"
#include "gameloop.h"
#include "rand.h"
#include "hunt_target.h"
#include "prob_density.h"
#include "gtk_callbacks.h"
#include "pcvspc.h"

/*  Este struct sera modificado por las funciones de configuracion contenidas
    en este archivo. Sirve para verificar si la configuracion es valida.
*/
struct
{
    int gm; //gamemode
    int frag, dest, acor, subm, port, tot;   //cantidad de barcos
    int shot;   //cantidad de turnos/disparos
    int start;  //jugador que inicia el juego
    int diff;   //dificultad
} config;

int barcos_posicionados;        //auxiliares para la colocacion de barcos
int aux_board[10][10] = {0};        //tablero auxiliar para la colocacion de barcos sin guardarse
int aux_orient = HORIZONTAL;
int aux_i = -10;
int aux_j = -10;

extern GtkWidget *window;

extern GtkDialog *config_dialog;  //dialogo de configuracion y componentes
extern GtkLabel *gamemode_sel;  //gamemode
extern GtkLabel *gamemode_label;
extern GtkLabel *config_warning;
extern GtkLabel *diff_select;  //difficulty
extern GtkLabel *diff_label;
extern GtkButton *diff_left;
extern GtkButton *diff_right;
extern GtkLabel *frag_sel;      //frag
extern GtkLabel *frag_label;
extern GtkLabel *dest_sel;      //dest
extern GtkLabel *dest_label;
extern GtkLabel *acor_sel;      //acor
extern GtkLabel *acor_label;
extern GtkLabel *subm_sel;      //subm
extern GtkLabel *subm_label;
extern GtkLabel *port_sel;      //port
extern GtkLabel *port_label;
extern GtkLabel *shot_sel;      //shot
extern GtkLabel *shot_label;
extern GtkEntry *shot_entry;
extern GtkLabel *start_sel;     //start
extern GtkLabel *start_label;
extern GtkButton *start_left;
extern GtkButton *start_right;
extern GtkLabel *name_ju;      //nombres
extern GtkLabel *name_op;
extern GtkEntry *name_ju_entry;
extern GtkEntry *name_op_entry;

extern GtkDialog *instruc_dialog;  //dialogo de configuracion y componentes
extern GtkLabel *instruc_text;         //texto

extern GtkLabel *status_label;
extern GtkImage *images_ju[10][10];   //guarda las imagenes del tablero
extern GtkImage *images_op[10][10];   //guarda las imagenes del tablero
extern GtkWidget *board_ju;    //tableros
extern GtkWidget *board_op;
extern GtkLabel *p_board_label;
extern GtkLabel *c_board_label;

extern GtkGrid *p_boats_grid;   //visualizacion de flotas
extern GtkGrid *c_boats_grid;
extern GtkLabel *p_boats_label;
extern GtkLabel *c_boats_label;
extern GtkLabel *p_boats_tot;
extern GtkLabel *p_boats_tot_n;
extern GtkLabel *p_boats_frag;
extern GtkLabel *p_boats_dest;
extern GtkLabel *p_boats_acor;
extern GtkLabel *p_boats_subm;
extern GtkLabel *p_boats_port;
extern GtkLabel *c_boats_tot;
extern GtkLabel *c_boats_tot_n;
extern GtkLabel *c_boats_frag;
extern GtkLabel *c_boats_dest;
extern GtkLabel *c_boats_acor;
extern GtkLabel *c_boats_subm;
extern GtkLabel *c_boats_port;
extern GtkLabel *p_boats_turns;
extern GtkLabel *p_boats_turns1;
extern GtkLabel *p_boats_turns_n;
extern GtkImage *p_boats_images[20];
extern GtkImage *c_boats_images[20];

extern GtkWidget *place_ship_config;
extern GtkLabel *ship_label;
extern GtkButton *orient_button;
extern GtkButton *place_ship_ok;
extern GtkButton *new_game_button;
extern GtkButton *start_button;

extern GtkDialog *game_over_dialog;    //dialogo de fin de juego
extern GtkLabel *game_over_victory;
extern GtkLabel *game_over_text;
extern GtkLabel *p1_name;          //estadisticas
extern GtkLabel *p2_name;
extern GtkLabel *p1_sunk;
extern GtkLabel *p2_sunk;
extern GtkLabel *p1_shots;
extern GtkLabel *p2_shots;
extern GtkLabel *p1_hits;
extern GtkLabel *p2_hits;
extern GtkLabel *p1_accuracy;
extern GtkLabel *p2_accuracy;

extern GtkDialog *about_dialog;        //dialogo acerca de

extern GtkDialog *stats_dialog;        //dialogo estadisticas
extern GtkLabel *stats_msg;    //por si no hay estadisticas
extern GtkGrid *stats_grid;

extern GtkDialog *path_dialog;     //dialogo para seleccion de directorio modo pvp
extern GtkFileChooserButton *path_file_chooser;
extern GtkLabel *path_msg;

//Texto del dialogo de instrucciones
char *instruc_body =
{
"REGLAS DE JUEGO:\n\n\
1) Colocacion de barcos:\n\
  - Barcos pueden ser colocados vertical u horizontalmente.\n\
  - Barcos no pueden solaparse (pero si tocarse).\n\n\
2) Disparos:\n\
  - Los jugadores toman turnos para disparar sobre el tablero opuesto.\n\
  - Si el disparo acierta, el disparador dispara de nuevo. La celda se\n\
    pinta con una cruz roja.\n\
  - Si el disparo falla, el siguiente jugador dispara. La celda se pinta\n\
    con un punto gris.\n\
  - Si se disparan todas las celdas de un barco, el barco se hunde. Las\n\
    del barco se pintan con una cruz gris.\n\n\
3) Objetivo:\n\
  - El jugador que logre hundir la flota enemiga gana.\n\
  - Si se opta por un limite de turnos, y si se alcanza, gana el jugador\n\
    que hunde mas barcos.\n\n\n\
CONFIGURACION DE UNA NUEVA PARTIDA:\n\n\
  - Elija el modo de juego y dificultad del oponente CPU, y la cantidad de\n\
    cada tipo de barco usando las flechas.\n\
    Obs.: Solo pueden seleccionarse entre 0 y 4 barcos del mismo tipo, y\n\
    no mas de 9 en total!\n\
  - Si desea, elija el limite de turnos. Si se deja en blanco se juega hasta\n\
    hundirse una flota.\n\
    Obs.: Solo hay 100 celdas en un tablero, asi que el limite debe ser de\n\
    entre 1 y 99 turnos. Otro numero no sera registrado!\n\
  - Elija quien comienza jugando usando las flechas.\n\
  - Ingrese su nombre para guardar sus estadisticas de juego.\n\n\n\
POSICIONAMIENTO DE FLOTA:\n\n\
  - Elija una posicion sobre su tablero con el raton. Barcos no pueden\n\
    solapar.\n\
    Obs.: Cada barco se diferencia por su inicial en el tablero.\n\
  - Cambie la orientacion usando el boton que indica la orientacion actual.\n\
    Obs.: Si el barco no aparece al accionar el raton sobre el tablero,\n\
    es porque no es una posicion valida. Prueba cambiar la orientacion!\n\
  - Cuando desee, usando el boton \"Colocar\", confirme su seleccion.\n\
    Obs.: El barco que se posiciona esta indicado encima del boton.\n\
  - Al terminar de posicionar su flota, comienze la partida.\n\n\n\
DESARROLLO DEL JUEGO:\n\n\
  - Dispare sobre el tablero del oponente usando el raton.\n\
    Obs.: El resultado de su disparo es inmediatamente visible:\n\
           Un punto gris indica un disparo fallido. Pasa el turno.\n\
           Una 'X' roja indica un acierto. Dispara otra vez.\n\
           Un grupo de 'X' grises indica que un barco se hundio en esas\n\
           posiciones.\n\
    Obs.: Revise el estado de cualquier flota debajo de su respectivo tablero\n\
    para adaptar su juego a los barcos faltantes!\n\n\n\
FIN DEL JUEGO:\n\
  - Si se hunde una flota o se alcanza el limite de turnos, el juego termina.\n\
  - Un mensaje anuncia el ganador y muestra estadisticas de la partida.\n\n\n\
NIVELES DE DIFICULTAD:\n\
  - Cadete: Realiza posicionamiento de barcos y disparos aleatorios.\n\
    No sabe planear sus jugadas, lo que lo hace un oponente trivial.\n\
  - Capitan: Realiza posicionamiento de barcos aleatorio, pero tiene cierta\n\
    estrategia al disparar. Opera en dos modos: Bucar e Interceptar. Al buscar\n\
    dispara aleatoriamente, al encontrar un barco lo intercepta.\n\
  - AI: Sigue una estrategia para el posicionamiento de su flota y\n\
    piensa sobre sus disparos. Tambien toma en cuenta los tipos de barcos\n\
    vivos para optimizar su busqueda.\n\n"
};

//Texturas para los tableros
#ifdef _WIN32
    char *img[] = {"img\\cell_empty.png",   //0
                   "img\\cell_miss.png",    //1
                   "img\\cell_ship.png",    //2
                   "img\\cell_hit.png",     //3
                   "img\\cell_sunk.png",    //4
                   "img\\cell_frag.png",    //5
                   "img\\cell_dest.png",    //6
                   "img\\cell_acor.png",    //7
                   "img\\cell_subm.png",    //8
                   "img\\cell_port.png",    //9
                   "img\\frag_icon.png",    //10
                   "img\\dest_icon.png",    //11
                   "img\\acor_icon.png",    //12
                   "img\\subm_icon.png",    //13
                   "img\\port_icon.png",    //14
                   "img\\sunk_icon.png",    //15
                   "img\\about_title.png",  //16
                   "img\\one.png",          //17
                   "img\\two.png",          //18
                   "img\\three.png",        //19
                   "img\\four.png",         //20
                   "img\\five.png",         //21
                   "img\\six.png",          //22
                   "img\\seven.png",        //23
                   "img\\eight.png",        //24
                   "img\\nine.png",         //25
                   "img\\ten.png"           //26
                   };
#endif // _WIN32
#ifdef __linux__
    char *img[] = {"img/cell_empty.png",   //0
                   "img/cell_miss.png",    //1
                   "img/cell_ship.png",    //2
                   "img/cell_hit.png",     //3
                   "img/cell_sunk.png",    //4
                   "img/cell_frag.png",    //5
                   "img/cell_dest.png",    //6
                   "img/cell_acor.png",    //7
                   "img/cell_subm.png",    //8
                   "img/cell_port.png",    //9
                   "img/frag_icon.png",    //10
                   "img/dest_icon.png",    //11
                   "img/acor_icon.png",    //12
                   "img/subm_icon.png",    //13
                   "img/port_icon.png",    //14
                   "img/sunk_icon.png",    //15
                   "img/about_title.png",  //16
                   "img/one.png",          //17
                   "img/two.png",          //18
                   "img/three.png",        //19
                   "img/four.png",         //20
                   "img/five.png",         //21
                   "img/six.png",          //22
                   "img/seven.png",        //23
                   "img/eight.png",        //24
                   "img/nine.png",         //25
                   "img/ten.png"           //26
                   };
#endif // __linux__

//Lista de mensajes para status_label
char *status[] = {"Posicione su barco:",                    //0
                  "Seleccione una posicion valida!",        //1
                  "Comienze la partida!",                   //2
                  "Su turno!",                              //3
                  "Turno del oponente!",                    //4
                  "Celda ya disparada!",                    //5
                  "Acierto! Su turno!",                     //6
                  "Acierto! Turno del oponente!",           //7
                  "Dispare sobre el tablero del oponente!", //8
                  "Turno de %s!",                           //9
                  "Error en el formato de historial.txt"    //10
                  };

//Lista de mensajes para el fin del juego
char *game_over[] = {"%s gana!",                                    //0
                     "%s gana!",                                    //1
                     "Empate!",                                     //2
                     "Flota enemiga destruida!",                    //3
                     "Flota aliada destruida!",                     //4
                     "%s ha hundido mas barcos!",                   //5
                     "%s ha hundido mas barcos!",                   //6
                     "Se hundieron la misma cantidad de barcos!",   //7
                     "No se hundieron barcos!"                      //8
                    };

extern int fase;
extern int turn;
extern int gamemode;
extern int difficulty;
extern int limite_disparos;
extern int lives_1;
extern int lives_2;
extern struct ship *fleet_1;
extern struct ship *fleet_2;

extern int tot_p1_shots;
extern int tot_p1_hits;
extern int tot_p2_shots;
extern int tot_p2_hits;

int turnos = 0;

gchar *p_name;
gchar *c_name;
gchar *def_name = "Jugador";
const char *stats_filename = "p_stats.bn";  //nombre del archivo de estadisticas

struct stats_row *row = NULL;
int n_rows = 0;

//funcion usada antes de su implementacion
void update_board_images();
void update_ships_images();
void place_ship_gui(int i, int j);

void change_status(int status_no)
{
    /* Cambia el status_label para mostrar el mensaje indicado por status_no */

    if (status_no == 9)
    {
        char str[30];
        if (turn == PLAYER_1)
        {
            snprintf(str, 30, status[9], p_name);
        } else
        {
            snprintf(str, 30, status[9], c_name);
        }

        gtk_label_set_text(status_label, str);
    } else
        gtk_label_set_text(status_label, status[status_no]);
}

void confirm_quit(GtkWidget *widget, gpointer data)
{
    /* Si el juego esta en progreso, pregunta por una confirmacion antes de salir del programa.
       Se llama al usar la opcion de menu "Salir".
    */

    GtkWidget *quit_dialog;

    if (fase == 0 || fase == 1)
    {
        quit_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                    GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_QUESTION,
                    GTK_BUTTONS_OK_CANCEL, "Una partida esta en progreso\nSalir?");

        if (gtk_dialog_run(GTK_DIALOG(quit_dialog)) == GTK_RESPONSE_OK)
			gtk_main_quit();

        gtk_widget_destroy(GTK_WIDGET(quit_dialog));
    } else
    {
        gtk_main_quit();
    }
}

gboolean confirm_quit_delete(GtkWidget *widget, GdkEvent  *event, gpointer data)
{
    /* Si el juego esta en progreso, pregunta por una confirmacion antes de salir del programa.
       Se llama al crear el evento "delete-event" en window.
    */

    GtkDialog *quit_dialog;

    if (fase == 0 || fase == 1)
    {
        quit_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                    GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_QUESTION,
                    GTK_BUTTONS_OK_CANCEL, "Una partida esta en progreso\nSalir?");

        if (gtk_dialog_run(GTK_DIALOG(quit_dialog)) == GTK_RESPONSE_OK)
        {
            gtk_widget_destroy(GTK_WIDGET(quit_dialog));
            return FALSE;
        }

        gtk_widget_destroy(GTK_WIDGET(quit_dialog));
        return TRUE;
    } else
    {
        return FALSE;
    }
}

/* Funciones de configuracion */
void run_config()
{
    /*  Funcion callback para mostrar el dialogo de configuracion de partida */

    gtk_dialog_run(config_dialog);
    gtk_widget_hide(GTK_WIDGET(config_dialog));
}

void init_config()
{
    /* Carga la configuracion inicial y modifica los labels */

    //Config
    config.gm = JVP;
    config.frag = 0;
    config.dest = 1;
    config.acor = 1;
    config.subm = 1;
    config.port = 2;
    config.tot = 5;
    config.shot = 100;
    config.start = PLAYER_1;
    config.diff = 1;

    //Labels
    gtk_label_set_text(gamemode_sel, "Modo de juego:");
    gtk_label_set_text(gamemode_label, "Jugador vs. CPU");
    gtk_label_set_text(diff_select, "Dificultad:");
    gtk_label_set_text(diff_label, "Capitan");   //tooltips
    gtk_label_set_text(frag_sel, "Fragatas:");
    gtk_label_set_text(frag_label, "0");
    gtk_label_set_text(dest_sel, "Destructores:");
    gtk_label_set_text(dest_label, "1");
    gtk_label_set_text(acor_sel, "Acorazados:");
    gtk_label_set_text(acor_label, "1");
    gtk_label_set_text(subm_sel, "Submarinos:");
    gtk_label_set_text(subm_label, "1");
    gtk_label_set_text(port_sel, "Portaviones:");
    gtk_label_set_text(port_label, "2");
    gtk_label_set_text(shot_sel, "Turnos:");
    gtk_label_set_text(shot_label, "Sin limite");
    gtk_entry_set_text(shot_entry, "");
    gtk_label_set_text(start_sel, "Comienza:");
    gtk_label_set_text(start_label, "Jugador");
    gtk_label_set_text(config_warning, "");
    gtk_button_set_label(orient_button, "Horizontal");
    gtk_label_set_text(name_ju, "Nombre del jugador");
    gtk_label_set_text(name_op, "Nombre del oponente");
    gtk_entry_set_text(name_ju_entry, "");
    gtk_entry_set_text(name_op_entry, "");
    p_name = "Jugador";
    c_name = "CPU";

    gtk_widget_hide(GTK_WIDGET(start_button));
    gtk_widget_hide(GTK_WIDGET(orient_button));
    gtk_widget_hide(GTK_WIDGET(place_ship_ok));
    gtk_widget_hide(GTK_WIDGET(status_label));

    set_fleetsize(config.frag, config.dest, config.acor, config.subm, config.port, config.tot);
    gamemode = config.gm;
    limite_disparos = config.shot;
    turn = config.start;
    barcos_posicionados = 0;
    change_status(0);

    tot_p1_shots = 0;
    tot_p1_hits = 0;
    tot_p2_shots = 0;
    tot_p2_hits = 0;
}

void gamemode_cb(GtkWidget *widget, gpointer data)
{
    /* Cambia el gamemode seleccionado en el dialogo de configuracion */

    if (config.gm == JVP)
    {
        config.gm = PVP;
        gtk_label_set_text(gamemode_label, "PC vs. PC");
        gtk_widget_set_sensitive(GTK_WIDGET(diff_left), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(diff_right), FALSE);
        config.diff = 2;
        gtk_label_set_text(diff_label, "AI");
        config.start = PLAYER_1;
        gtk_label_set_text(start_label, "Jugador");
    } else if (config.gm == PVP)
    {
        config.gm = JVP;
        gtk_label_set_text(gamemode_label, "Jugador vs. PC");
        gtk_widget_set_sensitive(GTK_WIDGET(diff_left), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(diff_right), TRUE);
    }
}

void difficulty_cb(GtkWidget *widget, gpointer data)
{
    /* Callback para diff_sel en en config */

    if (data == diff_left)
    {
        if (config.diff == 0)
        {
            config.diff = 2;
            gtk_label_set_text(diff_label, "AI");
        }
        else
        {
            config.diff--;
            if (config.diff == 0)
                gtk_label_set_text(diff_label, "Cadete");
            else
                gtk_label_set_text(diff_label, "Capitan");
        }
    } else
    {
        if (config.diff == 2)
        {
            config.diff = 0;
            gtk_label_set_text(diff_label, "Cadete");
        }
        else
        {
            config.diff++;
            if (config.diff == 2)
                gtk_label_set_text(diff_label, "AI");
            else
                gtk_label_set_text(diff_label, "Capitan");
        }
    }
}

void ship_add(GtkWidget *widget, gpointer data)
{
    /* Aumenta el contador del barco especificado en el dialogo de configuracion */
    char cant[2];
    int ship;
    cant[1] = '\0';

    if (data == NULL)
        return;
    else
        ship = data;

    switch (ship)
    {
    case FRAG:
        if (config.frag == 4)
            return;
        else
        {
            config.frag++;
            cant[0] = (char) (config.frag + '0');
            gtk_label_set_text(frag_label, cant);
        }
        break;

    case DEST:
        if (config.dest == 4)
            return;
        else
        {
            config.dest++;
            cant[0] = (char) (config.dest + '0');
            gtk_label_set_text(dest_label, cant);
        }
        break;

    case ACOR:
        if (config.acor == 4)
            return;
        else
        {
            config.acor++;
            cant[0] = (char) (config.acor + '0');
            gtk_label_set_text(acor_label, cant);
        }
        break;

    case SUBM:
        if (config.subm == 4)
            return;
        else
        {
            config.subm++;
            cant[0] = (char) (config.subm + '0');
            gtk_label_set_text(subm_label, cant);
        }
        break;

    case PORT:
        if (config.port == 4)
            return;
        else
        {
            config.port++;
            cant[0] = (char) (config.port + '0');
            gtk_label_set_text(port_label, cant);
        }
        break;
    }

    //Suma al total si se ejecuta toda la funcion
    config.tot++;
}

void ship_sub(GtkWidget *widget, gpointer data)
{
    /* Decrementa el contador del barco especificado en el dialogo de configuracion */
    char cant[2];
    int ship;
    cant[1] = '\0';

    if (data == NULL)
        return;
    else
        ship = data;

    switch (ship)
    {
    case FRAG:
        if (config.frag == 0)
            return;
        else
        {
            config.frag--;
            cant[0] = (char) (config.frag + '0');
            gtk_label_set_text(frag_label, cant);
        }
        break;

    case DEST:
        if (config.dest == 0)
            return;
        else
        {
            config.dest--;
            cant[0] = (char) (config.dest + '0');
            gtk_label_set_text(dest_label, cant);
        }
        break;

    case ACOR:
        if (config.acor == 0)
            return;
        else
        {
            config.acor--;
            cant[0] = (char) (config.acor + '0');
            gtk_label_set_text(acor_label, cant);
        }
        break;

    case SUBM:
        if (config.subm == 0)
            return;
        else
        {
            config.subm--;
            cant[0] = (char) (config.subm + '0');
            gtk_label_set_text(subm_label, cant);
        }
        break;

    case PORT:
        if (config.port == 0)
            return;
        else
        {
            config.port--;
            cant[0] = (char) (config.port + '0');
            gtk_label_set_text(port_label, cant);
        }
        break;
    }

    //Resta al total si se ejecuta toda la funcion
    config.tot--;
}

int get_shots()
{
    /* Verifica que la entrada a shot_entry sea valida, es decir, no hay caracteres no numericos.
        Retorno:
            shots: si es valido
            -1: si no es valido
    */

    const gchar *str;
    int shots;

    str = gtk_entry_get_text(shot_entry);

    //conversion de gchar a int
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] < '0' || str[i] > '9')
            return -1;

    shots = atoi((const char*) str);
    return shots;
}

void shot_cb(GtkWidget *widget, gpointer data)
{
    /* Callback para shot_entry */

    int shots;
    const gchar *buffer;

    if ((shots = get_shots()) == -1)
    {
        config.shot = -1;
        gtk_label_set_text(shot_label, "<Caracter no valido>");
    } else if (shots == 0 || shots > 100)
    {
        config.shot = 100;
        gtk_label_set_text(shot_label, "Sin limite");
    } else
    {
        config.shot = shots;
        if (shots == 100)
        {
            gtk_label_set_text(shot_label, "Sin limite");
        } else
        {
            buffer = gtk_entry_get_text(shot_entry);
            gtk_label_set_text(shot_label, buffer);
        }
    }
}

void start_cb(GtkWidget *widget, gpointer data)
{
    /* Cambia el jugador que comienza el juego */

    if (data == start_left)
    {
        if (config.start == -1)
        {
            config.start = PLAYER_2;
            if (config.gm == JVP)
                gtk_label_set_text(start_label, "PC");
            else
                gtk_label_set_text(start_label, "Oponente");
        } else
        {
            config.start--;
            if (config.start == PLAYER_1)
                gtk_label_set_text(start_label, "Jugador");
            else
            {
                if (config.gm == 1)
                {
                    config.start = PLAYER_2;
                    gtk_label_set_text(start_label, "Oponente");
                } else
                    gtk_label_set_text(start_label, "Aleatorio");
            }
        }
    } else if (data == start_right)
    {
        if (config.start == PLAYER_2)
        {
            if (config.gm == 0)
            {
                config.start = -1;
                gtk_label_set_text(start_label, "Aleatorio");
            } else
            {
                config.start = 0;
                gtk_label_set_text(start_label, "Jugador");
            }
        } else
        {
            config.start++;
            if (config.start == PLAYER_1)
                gtk_label_set_text(start_label, "Jugador");
            else
                if (config.gm == JVP)
                    gtk_label_set_text(start_label, "PC");
                else
                    gtk_label_set_text(start_label, "Oponente");
        }
    }
}

int chk_config()
{
    /* Verifica si la configuracion es valida.
        Retorno:
            1: valida
            0: no valida
    */

    //Modo de juego
    if (config.gm == 1)
    {
        if (strlen(gtk_entry_get_text(name_ju_entry)) == 0)
        {
            gtk_label_set_text(config_warning, "Nombre del jugador vacio!");
            return 0;
        } else if (strlen(gtk_entry_get_text(name_op_entry)) == 0)
        {
            gtk_label_set_text(config_warning, "Nombre del oponente vacio!");
            return 0;
        } else if (strcmp(gtk_entry_get_text(name_ju_entry), gtk_entry_get_text(name_op_entry)) == 0)
        {
            gtk_label_set_text(config_warning, "Nombres no pueden ser iguales!");
            return 0;
        }
    }

    //Cantidad de barcos
    if (config.tot == 0)
    {
        gtk_label_set_text(config_warning, "No hay barcos!");
        return 0;
    } else if (config.tot > 9)
    {
        gtk_label_set_text(config_warning, "Demasiados barcos! (max. 9)");
        return 0;
    }

    //Limite de turnos
    if (get_shots() == -1)
    {
        gtk_label_set_text(config_warning, "Limite invalido!");
        return 0;
    }

    //Config. valida
    gtk_label_set_text(config_warning, "");
    return 1;
}

void config_ok_cb(GtkWidget *widget, gpointer data)
{
    /* Callback para el boton OK en config_dialog. Caso la configuracion sea valida,
       se aplica y se cierra el dialogo.
    */

    int valid;

    valid = chk_config();

    if (!valid)
    {
        return;
    } else if (valid)
    {
        update_board_images();
        update_ships_images();

        set_fleetsize(config.frag, config.dest, config.acor, config.subm, config.port, config.tot);
        gamemode = config.gm;
        difficulty = config.diff;
        limite_disparos = config.shot;

        if (config.start == -1)
            turn = rand() % 2;
        else
            turn = config.start;

        fase = 0;

        gtk_widget_show(GTK_WIDGET(orient_button));
        gtk_widget_show(GTK_WIDGET(place_ship_ok));
        gtk_widget_show(GTK_WIDGET(status_label));
        gtk_widget_hide(GTK_WIDGET(new_game_button));

        if (config.frag > 0)
            gtk_label_set_text(ship_label, "Fragata");
        else if (config.dest > 0)
            gtk_label_set_text(ship_label, "Destructor");
        else if (config.acor > 0)
            gtk_label_set_text(ship_label, "Acorazado");
        else if (config.subm > 0)
            gtk_label_set_text(ship_label, "Submarino");
        else
            gtk_label_set_text(ship_label, "Portaviones");

        p_name = gtk_entry_get_text(name_ju_entry);
        if (p_name[0] == '\0')
        {
            p_name = "Jugador";
        }

        c_name = gtk_entry_get_text(name_op_entry);
        if (c_name[0] == '\0')
        {
            if (difficulty == 0)
                c_name = "Cadete Randall";
            else if (difficulty == 1)
                c_name = "Capitan Hunter";
            else
                c_name = "S.H.I.P.";    //Super High Intelligence Peashooter
        }

        gtk_label_set_text(p_board_label, p_name);
        gtk_label_set_text(c_board_label, c_name);

        gtk_dialog_response(config_dialog, GTK_RESPONSE_OK);

        if (gamemode == 1)  //pvp
        {
            run_path();

            //posicionamiento de barcos
            pvp_setup_board();
            fase = 1;
        }
    }
}

void config_reset_cb()
{
    /* Callback para el boton config_reset */

    init_config();
}

/* Funciones de path */
void run_path()
{
    /* Muestra el dialogo path_dialog */
    gtk_label_set_text(path_msg, "");
    gtk_dialog_run(path_dialog);
    gtk_widget_grab_focus(GTK_WIDGET(path_file_chooser));
    gtk_widget_hide(GTK_WIDGET(path_dialog));
}

void path_ok_cb()
{
    /* Callback para path_ok. Verifica que un directorio se selecciona */

    gchar *folder;

    folder = gtk_file_chooser_get_uri(path_file_chooser);

    if (folder == NULL)
    {
        gtk_label_set_text(path_msg, "Seleccione un directorio!");
        return;
    } else
    {
        pvp_set_path(folder);
        g_free(folder);
        gtk_dialog_response(path_dialog, GTK_RESPONSE_APPLY);
    }
}

/* Funciones de intrucciones */
void init_instruc()
{
    /* Inicializa el dialogo instrucciones */

    gtk_label_set_text(instruc_text, instruc_body);
}

void run_instruc(GtkWidget *widget, gpointer data)
{
    /*  Funcion callback para mostrar el dialogo de instrucciones */

    init_instruc();
    gtk_dialog_run(instruc_dialog);
    gtk_widget_hide(GTK_WIDGET(instruc_dialog));
}

void instruc_ok_cb(GtkWidget *widget, gpointer data)
{
    /* Cierra el cuadro de dialogo de instrucciones */
    gtk_dialog_response(instruc_dialog, GTK_RESPONSE_OK);
}

/* Funciones de game_over */
void game_over_stats()
{
    /* Escribe las extadisticas de la partida sobre el game_over_dialog */

    //calculo de estadisticas
    int p1_s = config.tot - lives_2;
    int p2_s = config.tot - lives_1;
    float p1_a = (tot_p1_hits / (float) tot_p1_shots) * 100;
    float p2_a = (tot_p2_hits / (float) tot_p2_shots) * 100;

    char p1_sc[2];
    char p2_sc[2];
    char p1_shc[4];
    char p2_shc[4];
    char p1_hc[3];
    char p2_hc[3];
    char p1_ac[7];
    char p2_ac[7];

    //creacion de cadenas
    snprintf(p1_sc, 2, "%d", p1_s);
    snprintf(p2_sc, 2, "%d", p2_s);

    snprintf(p1_shc, 4, "%d", tot_p1_shots);
    snprintf(p2_shc, 4, "%d", tot_p2_shots);

    snprintf(p1_hc, 3, "%d", tot_p1_hits);
    snprintf(p2_hc, 3, "%d", tot_p2_hits);

    snprintf(p1_ac, 7, "%1.1f %%", p1_a);
    snprintf(p2_ac, 7, "%1.1f %%", p2_a);

    //escritura
    gtk_label_set_text(p1_name, p_name);
    gtk_label_set_text(p2_name, c_name);
    gtk_label_set_text(p1_sunk, p1_sc);
    gtk_label_set_text(p2_sunk, p2_sc);
    gtk_label_set_text(p1_shots, p1_shc);
    gtk_label_set_text(p2_shots, p2_shc);
    gtk_label_set_text(p1_hits, p1_hc);
    gtk_label_set_text(p2_hits, p2_hc);
    gtk_label_set_text(p1_accuracy, p1_ac);
    gtk_label_set_text(p2_accuracy, p2_ac);
}

long file_len(FILE * f)
{
    /* Retorna la longitud del archivo f */

    long cur;
    long ret;

    if (f == NULL)
    {
        return 0;
    } else
    {
        cur = ftell(f);         //guarda la posicion inicial
        fseek(f, 0, SEEK_END);
        ret = ftell(f);
        fseek(f, 0, SEEK_SET);  //pone al puntero en su posicion inicial
        return ret;
    }
}

void save_player_stats()
{
    /* Guarda las estadisticas del juego de manera permanente */

    FILE * f = NULL;
    struct player_stats *log;   //puntero al que se copia el archivo
    struct player_stats new_player; //nuevo registro
    long len;
    int cant;
    int exists = 1;
    int i;

    //si no existe, f = NULL
    f = fopen(stats_filename, "rb+");

    //lectura
    if (f != NULL)  //el archivo existe
    {
        len = file_len(f);
        cant = len/sizeof(struct player_stats); //cantidad de jugadores guardados

        log = (struct player_stats*) malloc(sizeof(struct player_stats)*(cant+1));  //cant+1 para poder guardar un elemento nuevo

        for (i = 0; i < cant; i++)
        {
            fread(&log[i], sizeof(struct player_stats), 1, f);
        }

        if (strcmp(p_name, def_name))  //si se introdujo un nombre
        {
            for (i = 0; i < cant; i++)  //itera todos los jugadores guardados
            {
                if ((exists = strcmp(p_name, log[i].name)) == 0)
                {
                    break;
                }
            }
            //if exists != 0, name is new, else, name already exists in file

            //write player stats
            if (exists == 0) //i es el indice del struct del mismo nombre
            {
                if (winner() == PLAYER_1)
                {
                    log[i].won++;
                } else if (winner() == PLAYER_2)
                {
                    log[i].lost++;
                } else
                {
                    log[i].tied++;
                }
                log[i].played++;
            } else
            {
                //cant es la ultima posicion, que fue alocada para guardar un nombre nuevo
                strcpy(new_player.name, p_name);

                new_player.played = 1;

                if (winner() == PLAYER_1)
                {
                    new_player.won = 1;
                    new_player.lost = 0;
                    new_player.tied = 0;
                } else if (winner() == PLAYER_2)
                {
                    new_player.won = 0;
                    new_player.lost = 1;
                    new_player.tied = 0;
                } else
                {
                    new_player.won = 0;
                    new_player.lost = 0;
                    new_player.tied = 1;
                }

                log[cant] = new_player;
                cant++; //se incrementa cant para escribirse en el archivo
            }

            fflush(f);
            fseek(f, 0, SEEK_SET);
            fwrite(log, sizeof(struct player_stats), cant, f);
        }
        free(log);
    } else  //se crea un archivo nuevo
    {
        f = fopen(stats_filename, "wb");

        //creacion de nuevo struct player_stats
        new_player.played = 1;

        if (winner() == PLAYER_1)
        {
            new_player.won = 1;
            new_player.lost = 0;
            new_player.tied = 0;
        } else if (winner() == PLAYER_2)
        {
            new_player.won = 0;
            new_player.lost = 1;
            new_player.tied = 0;
        } else
        {
            new_player.won = 0;
            new_player.lost = 0;
            new_player.tied = 1;
        }

        if (strcmp(p_name, def_name))
        {
            strcpy(new_player.name, p_name);
            fwrite(&new_player, sizeof(struct player_stats), 1, f);
        }
    }

    fclose(f);
}

void run_game_over()
{
    /* Muestra el dialogo de fin de juego */

    end_game();
    if (gamemode == 0)
        save_player_stats();
    game_over_stats();
    gtk_dialog_run(game_over_dialog);
    gtk_widget_hide(GTK_WIDGET(game_over_dialog));
}

void game_over_new_cb()
{
    /* Callback para game_over_new. Reinicia el juego */

    new_game();
    gtk_dialog_response(game_over_dialog, GTK_RESPONSE_ACCEPT);
}

void game_over_quit_cb()
{
    /* Callback para game_over_quit. Cierra el programa */

    gtk_dialog_response(game_over_dialog, GTK_RESPONSE_CLOSE);
    gtk_main_quit();
}

/* Funciones de about */
void run_about()
{
    /* Muestra el dialogo about */
    gtk_dialog_run(about_dialog);
    gtk_widget_hide(GTK_WIDGET(about_dialog));
}

/* Funciones de stats */
void run_stats()
{
    /* Muestra el dialogo stats */
    show_stats();
    gtk_dialog_run(stats_dialog);
    gtk_widget_hide(GTK_WIDGET(stats_dialog));
}

void show_stats()
{
    /* Muestra estadisticas de jugadores guardadas, si existen */

    FILE * f;
    long len;
    int cant;
    struct player_stats *log;
    struct player_stats aux;
//    int n_rows = 0;        //para evitar crear labels innecesarios

    char played[5]; //para poder poner numeros en forma de texto sobre labels
    char won[5];
    char lost[5];
    char tied[5];
    char win_ratio[10];
    float w_r;

    f = fopen(stats_filename, "rb");

    len = file_len(f);
    cant = len/sizeof(struct player_stats);

    log = (struct player_stats*) malloc(sizeof(struct player_stats)*cant);

    if (f == NULL || len == 0)
    {
        gtk_label_set_text(stats_msg, "No hay estadisticas guardadas!\nJuege una partida con su nombre.");
    } else
    {
        gtk_label_set_text(stats_msg, "");

        for (int i = 0; i < cant; i++)
        {
            fread(&log[i], sizeof(struct player_stats), 1, f);
        }

        //sort log (bubble sort)
        for (int i = 0; i < cant-1; i++)
        {
            for (int j = 0; j < cant-1-i; j++)
            {
                if (log[j].won < log[j+1].won)  //menos ganadas -> mayor indice
                {
                    aux = log[j];
                    log[j] = log[j+1];
                    log[j+1] = aux;
                }
            }
        }

        for (int i = 0; i < cant; i++)
        {
            w_r = ((float) log[i].won / (float) log[i].played) *100;

            snprintf(played, 5, "%d", log[i].played);
            snprintf(won, 5, "%d", log[i].won);
            snprintf(win_ratio, 10, "%.1f %%", w_r);
            snprintf(lost, 5, "%d", log[i].lost);
            snprintf(tied, 5, "%d", log[i].tied);

            if (n_rows > i)
            {
                gtk_label_set_text(row[i].name, log[i].name);
                gtk_label_set_text(row[i].played, played);
                gtk_label_set_text(row[i].won, won);
                gtk_label_set_text(row[i].win_ratio, win_ratio);
                gtk_label_set_text(row[i].lost, lost);
                gtk_label_set_text(row[i].tied, tied);
            } else  //hay menos filas que jugadores guardados
            {
                n_rows++;
                row = realloc(row, sizeof(struct stats_row)*(n_rows));

                row[n_rows-1].name = GTK_LABEL(gtk_label_new(log[i].name));
                gtk_widget_set_margin_bottom(GTK_WIDGET(row[n_rows-1].name), 5);
                row[n_rows-1].played = GTK_LABEL(gtk_label_new(played));
                row[n_rows-1].won = GTK_LABEL(gtk_label_new(won));
                row[n_rows-1].win_ratio = GTK_LABEL(gtk_label_new(win_ratio));
                row[n_rows-1].lost = GTK_LABEL(gtk_label_new(lost));
                row[n_rows-1].tied = GTK_LABEL(gtk_label_new(tied));

                gtk_grid_attach(stats_grid, row[n_rows-1].name, 0, n_rows, 1, 1);
                gtk_grid_attach(stats_grid, row[n_rows-1].played, 1, n_rows, 1, 1);
                gtk_grid_attach(stats_grid, row[n_rows-1].won, 2, n_rows, 1, 1);
                gtk_grid_attach(stats_grid, row[n_rows-1].win_ratio, 3, n_rows, 1, 1);
                gtk_grid_attach(stats_grid, row[n_rows-1].lost, 4, n_rows, 1, 1);
                gtk_grid_attach(stats_grid, row[n_rows-1].tied, 5, n_rows, 1, 1);
            }
        }
    }

    gtk_widget_show_all(GTK_WIDGET(stats_grid));
    free(log);
    fclose(f);
}

void stats_close_cb(GtkWidget *widget, gpointer data)
{
    /* Cierra el dialogo stats */
    gtk_dialog_response(stats_dialog, GTK_RESPONSE_OK);
}

/* Funciones de tableros y display */
void update_board_images()
{
    /* Reasigna imagenes de acuerdo al tablero interno al tablero de la interfaz grafica */

    extern int fleetb_1[10][10];
    extern int enemyb_1[10][10];
    extern int mystery_board[10][10];

    if (fase == 0)  //colocacion de barcos
    {
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                switch (aux_board[i][j])
                {
                case AGUA:
                    gtk_image_set_from_file(images_ju[i][j], img[0]);
                    break;
                case FRAG:
                    gtk_image_set_from_file(images_ju[i][j], img[5]);
                    break;
                case DEST:
                    gtk_image_set_from_file(images_ju[i][j], img[6]);
                    break;
                case ACOR:
                    gtk_image_set_from_file(images_ju[i][j], img[7]);
                    break;
                case SUBM:
                    gtk_image_set_from_file(images_ju[i][j], img[8]);
                    break;
                case PORT:
                    gtk_image_set_from_file(images_ju[i][j], img[9]);
                    break;
                }
            }
        }
    } else if (fase == 1)   //disparos
    {
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                switch (fleetb_1[i][j]) //tablero jugador
                {
                case AGUA:
                    gtk_image_set_from_file(images_ju[i][j], img[0]);
                    break;
                case FRAG:
                    gtk_image_set_from_file(images_ju[i][j], img[5]);
                    break;
                case DEST:
                    gtk_image_set_from_file(images_ju[i][j], img[6]);
                    break;
                case ACOR:
                    gtk_image_set_from_file(images_ju[i][j], img[7]);
                    break;
                case SUBM:
                    gtk_image_set_from_file(images_ju[i][j], img[8]);
                    break;
                case PORT:
                    gtk_image_set_from_file(images_ju[i][j], img[9]);
                    break;
                case HIT:
                    gtk_image_set_from_file(images_ju[i][j], img[3]);
                    break;
                case MISS:
                    gtk_image_set_from_file(images_ju[i][j], img[1]);
                    break;
                case SUNK:
                    gtk_image_set_from_file(images_ju[i][j], img[4]);
                    break;
                }

                if (gamemode == 0)  //jvp
                {
                    switch (enemyb_1[i][j]) //tablero disparos
                    {
                    case FRAG: case DEST: case ACOR: case SUBM: case PORT:
                    case AGUA:
                        gtk_image_set_from_file(images_op[i][j], img[0]);
                        break;
                    case HIT:
                        gtk_image_set_from_file(images_op[i][j], img[3]);
                        break;
                    case MISS:
                        gtk_image_set_from_file(images_op[i][j], img[1]);
                        break;
                    case SUNK:
                        gtk_image_set_from_file(images_op[i][j], img[4]);
                        break;
                    }
                } else  //pvp
                {
                    switch (mystery_board[i][j]) //tablero disparos
                    {
                    case AGUA:
                        gtk_image_set_from_file(images_op[i][j], img[0]);
                        break;
                    case HIT:
                        gtk_image_set_from_file(images_op[i][j], img[3]);
                        break;
                    case MISS:
                        gtk_image_set_from_file(images_op[i][j], img[1]);
                        break;
                    case SUNK:
                        gtk_image_set_from_file(images_op[i][j], img[4]);
                        break;
                    }
                }
            }
        }
    } else
        return;
}

void set_ships_images()
{
    /* Asigna imagenes de acuerdo a fleet_1 y fleet_2 al grid de la interfaz grafica */

    int i, index, ship_n, aux;
    extern struct ship* fleet_1;
    extern struct ship* fleet_2;
    extern int fleetsize[5];

    //labels
    gtk_label_set_text(p_boats_tot, "Total:");
    gtk_label_set_text(p_boats_tot_n, "");
    gtk_label_set_text(c_boats_tot, "Total:");
    gtk_label_set_text(c_boats_tot_n, "");

    gtk_label_set_text(p_boats_frag, "Fragatas: ");
    gtk_label_set_text(p_boats_dest, "Destructores: ");
    gtk_label_set_text(p_boats_acor, "Acorazados: ");
    gtk_label_set_text(p_boats_subm, "Submarinos: ");
    gtk_label_set_text(p_boats_port, "Portaviones: ");

    gtk_label_set_text(c_boats_frag, "Fragatas: ");
    gtk_label_set_text(c_boats_dest, "Destructores: ");
    gtk_label_set_text(c_boats_acor, "Acorazados: ");
    gtk_label_set_text(c_boats_subm, "Submarinos: ");
    gtk_label_set_text(c_boats_port, "Portaviones: ");

    gtk_label_set_text(p_boats_turns, "Quedan ");
    gtk_label_set_text(p_boats_turns1, " turnos");
    gtk_label_set_text(p_boats_turns_n, "100");

    //imagenes
    for (i = 0; i < 20; i++)
    {
        p_boats_images[i] = GTK_IMAGE(gtk_image_new());
        gtk_grid_attach(p_boats_grid, p_boats_images[i], (i % 4) + 1, (i / 4) + 1, 1, 1);
        c_boats_images[i] = GTK_IMAGE(gtk_image_new());
        gtk_grid_attach(c_boats_grid, c_boats_images[i], (i % 4) + 1, (i / 4) + 1, 1, 1);
    }

    gtk_widget_hide(GTK_WIDGET(p_boats_label));
    gtk_widget_hide(GTK_WIDGET(c_boats_label));
    gtk_widget_hide(GTK_WIDGET(p_boats_grid));
    gtk_widget_hide(GTK_WIDGET(c_boats_grid));
}

void update_ships_images()
{
    /* Reasigna imagenes de acuerdo a fleet_1 y fleet_2 al grid de la interfaz grafica */

    char n[2] = "n";
    char turn[4];
    int fr_n = 0;
    int de_n = 0;
    int ac_n = 0;
    int su_n = 0;
    int po_n = 0;

    extern int PD_fleetsize[5];

    gtk_widget_show(p_boats_label);
    gtk_widget_show(c_boats_label);
    gtk_widget_show(p_boats_grid);
    gtk_widget_show(c_boats_grid);

    if (config.shot == 100)
    {
        gtk_widget_hide(p_boats_turns);
        gtk_widget_hide(p_boats_turns1);
        gtk_widget_hide(p_boats_turns_n);
    } else
    {
        gtk_label_set_text(p_boats_turns, "Quedan ");
        gtk_label_set_text(p_boats_turns1, " turnos");
        snprintf(turn, 4, "%d", config.shot-turnos);
        gtk_label_set_text(p_boats_turns_n, turn);

        gtk_widget_show(p_boats_turns);
        gtk_widget_show(p_boats_turns1);
        gtk_widget_show(p_boats_turns_n);
    }

    n[0] = '0' + lives_1;
    gtk_label_set_text(p_boats_tot_n, n);
    n[0] = '0' + lives_2;
    gtk_label_set_text(c_boats_tot_n, n);

    if (config.frag == 0)
    {
        gtk_widget_hide(GTK_WIDGET(p_boats_frag));
        gtk_widget_hide(GTK_WIDGET(c_boats_frag));
    } else
    {
        gtk_widget_show(GTK_WIDGET(p_boats_frag));
        gtk_widget_show(GTK_WIDGET(c_boats_frag));
    }

    if (config.dest == 0)
    {
        gtk_widget_hide(GTK_WIDGET(p_boats_dest));
        gtk_widget_hide(GTK_WIDGET(c_boats_dest));
    } else
    {
        gtk_widget_show(GTK_WIDGET(p_boats_dest));
        gtk_widget_show(GTK_WIDGET(c_boats_dest));
    }

    if (config.acor == 0)
    {
        gtk_widget_hide(GTK_WIDGET(p_boats_acor));
        gtk_widget_hide(GTK_WIDGET(c_boats_acor));
    } else
    {
        gtk_widget_show(GTK_WIDGET(p_boats_acor));
        gtk_widget_show(GTK_WIDGET(c_boats_acor));
    }

    if (config.subm == 0)
    {
        gtk_widget_hide(GTK_WIDGET(p_boats_subm));
        gtk_widget_hide(GTK_WIDGET(c_boats_subm));
    } else
    {
        gtk_widget_show(GTK_WIDGET(p_boats_subm));
        gtk_widget_show(GTK_WIDGET(c_boats_subm));
    }

    if (config.port == 0)
    {
        gtk_widget_hide(GTK_WIDGET(p_boats_port));
        gtk_widget_hide(GTK_WIDGET(c_boats_port));
    } else
    {
        gtk_widget_show(GTK_WIDGET(p_boats_port));
        gtk_widget_show(GTK_WIDGET(c_boats_port));
    }

    for (int i = 0; i < 20; i++)
    {
        gtk_widget_hide(p_boats_images[i]);
        gtk_widget_hide(c_boats_images[i]);
    }

    for (int i = 0; i < config.tot; i++)
    {
        switch (fleet_1[i].size)
        {
        case 0:
            if (fleet_1[i].live > 0)
                gtk_image_set_from_file(p_boats_images[fr_n], img[10]);
            else
                gtk_image_set_from_file(p_boats_images[fr_n], img[15]);
            gtk_widget_show(p_boats_images[fr_n]);

            if (gamemode == 0)
            {
                if (fleet_2[i].live > 0)
                    gtk_image_set_from_file(c_boats_images[fr_n], img[10]);
                else
                    gtk_image_set_from_file(c_boats_images[fr_n], img[15]);
                gtk_widget_show(c_boats_images[fr_n]);
            } else
            {
                if (PD_fleetsize[0] > fr_n)
                    gtk_image_set_from_file(c_boats_images[fr_n], img[10]);
                else
                    gtk_image_set_from_file(c_boats_images[fr_n], img[15]);
                gtk_widget_show(c_boats_images[fr_n]);
            }

            fr_n++;
            break;
        case 1:
            if (fleet_1[i].live > 0)
                gtk_image_set_from_file(p_boats_images[4+de_n], img[11]);
            else
                gtk_image_set_from_file(p_boats_images[4+de_n], img[15]);
            gtk_widget_show(p_boats_images[4+de_n]);

            if (gamemode == 0)
            {
                if (fleet_2[i].live > 0)
                    gtk_image_set_from_file(c_boats_images[4+de_n], img[11]);
                else
                    gtk_image_set_from_file(c_boats_images[4+de_n], img[15]);
                gtk_widget_show(c_boats_images[4+de_n]);
            } else
            {
                if (PD_fleetsize[1] > de_n)
                    gtk_image_set_from_file(c_boats_images[4+de_n], img[11]);
                else
                    gtk_image_set_from_file(c_boats_images[4+de_n], img[15]);
                gtk_widget_show(c_boats_images[4+de_n]);
            }

            de_n++;
            break;
        case 2:
            if (fleet_1[i].live > 0)
                gtk_image_set_from_file(p_boats_images[4*2+ac_n], img[12]);
            else
                gtk_image_set_from_file(p_boats_images[4*2+ac_n], img[15]);
            gtk_widget_show(p_boats_images[4*2+ac_n]);

            if (gamemode == 0)
            {
                if (fleet_2[i].live > 0)
                    gtk_image_set_from_file(c_boats_images[4*2+ac_n], img[12]);
                else
                    gtk_image_set_from_file(c_boats_images[4*2+ac_n], img[15]);
                gtk_widget_show(c_boats_images[4*2+ac_n]);
            } else
            {
                if (PD_fleetsize[2] > ac_n)
                    gtk_image_set_from_file(c_boats_images[4*2+ac_n], img[12]);
                else
                    gtk_image_set_from_file(c_boats_images[4*2+ac_n], img[15]);
                gtk_widget_show(c_boats_images[4*2+ac_n]);
            }

            ac_n++;
            break;
        case 3:
            if (fleet_1[i].live > 0)
                gtk_image_set_from_file(p_boats_images[(4*3)+su_n], img[13]);
            else
                gtk_image_set_from_file(p_boats_images[(4*3)+su_n], img[15]);
            gtk_widget_show(p_boats_images[(4*3)+su_n]);

            if (gamemode == 0)
            {
                if (fleet_2[i].live > 0)
                    gtk_image_set_from_file(c_boats_images[(4*3)+su_n], img[13]);
                else
                    gtk_image_set_from_file(c_boats_images[(4*3)+su_n], img[15]);
                gtk_widget_show(c_boats_images[(4*3)+su_n]);
            } else
            {
                if (PD_fleetsize[3] > su_n)
                    gtk_image_set_from_file(c_boats_images[(4*3)+su_n], img[13]);
                else
                    gtk_image_set_from_file(c_boats_images[(4*3)+su_n], img[15]);
                gtk_widget_show(c_boats_images[(4*3)+su_n]);
            }

            su_n++;
            break;
        case 4:
            if (fleet_1[i].live > 0)
                gtk_image_set_from_file(p_boats_images[(4*4)+po_n], img[14]);
            else
                gtk_image_set_from_file(p_boats_images[(4*4)+po_n], img[15]);
            gtk_widget_show(p_boats_images[(4*4)+po_n]);

            if (gamemode == 0)
            {
                if (fleet_2[i].live > 0)
                    gtk_image_set_from_file(c_boats_images[(4*4)+po_n], img[14]);
                else
                    gtk_image_set_from_file(c_boats_images[(4*4)+po_n], img[15]);
                gtk_widget_show(c_boats_images[(4*4)+po_n]);
            } else
            {
                if (PD_fleetsize[4] > po_n)
                    gtk_image_set_from_file(c_boats_images[(4*4)+po_n], img[14]);
                else
                    gtk_image_set_from_file(c_boats_images[(4*4)+po_n], img[15]);
                gtk_widget_show(c_boats_images[(4*4)+po_n]);
            }

            po_n++;
            break;
        }
    }
}

void orient_button_cb()
{
    /* Cambia la orientacion seleccionada */

    if (fase != 0)
        return;

    if (aux_orient == HORIZONTAL)
    {
        aux_orient = VERTICAL;
        gtk_button_set_label(orient_button, "Vertical");
        place_ship_gui(aux_i, aux_j);
    }
    else if (aux_orient == VERTICAL)
    {
        aux_orient = HORIZONTAL;
        gtk_button_set_label(orient_button, "Horizontal");
        place_ship_gui(aux_i, aux_j);
    }
}

void place_ship_gui(int x, int y)
{
    /* Posiciona los barcos del jugador en las posiciones que elige si hacer los cambios definitivos.
       La idea es poder visualizar las posiciones pero posibilitar cambios.
    */

    int size;

    extern int fleetb_1[10][10];

    if (fase > 0)
        return;

    for (int i = 0; i < 10; i++)    //se copia el tablero real al auxiliar para poder modificarse sin problemas
    {
        for (int j = 0; j < 10; j++)
        {
            aux_board[i][j] = fleetb_1[i][j];
        }
    }

    if (barcos_posicionados - config.frag < 0)  //tamanho del barco actual
        size = 0;
    else if (barcos_posicionados - config.frag - config.dest < 0)
        size = 1;
    else if (barcos_posicionados - config.frag - config.dest - config.acor < 0)
        size = 2;
    else if (barcos_posicionados - config.frag - config.dest  - config.acor - config.subm < 0)
        size = 3;
    else if (barcos_posicionados - config.tot < 0)
        size = 4;
    else
        return;

    place_ship(-1, size, 0, aux_orient, x, y);
    update_board_images();
    aux_i = x;
    aux_j = y;
}

void place_ship_ok_cb()
{
    /* Confirma la seleccion de posicion y la guarda en la matriz interna */

    extern int fleetb_1[10][10];
    int size;
    int ship_n;
    int aux;

    if (fase > 0)
        return;
    else if ((aux_i < 0 || aux_i > 9 || aux_j < 0 || aux_j > 9) && barcos_posicionados < config.tot)
    {
        change_status(1);
        return;
    } else if (barcos_posicionados == config.tot)
    {
        change_status(2);
        return;
    }

    if (barcos_posicionados - config.frag < 0)  //tamanho del barco actual
        size = 0;
    else if (barcos_posicionados - config.frag - config.dest < 0)
        size = 1;
    else if (barcos_posicionados - config.frag - config.dest - config.acor < 0)
        size = 2;
    else if (barcos_posicionados - config.frag - config.dest  - config.acor - config.subm < 0)
        size = 3;
    else if (barcos_posicionados - config.tot < 0)
        size = 4;
    else
        return;

    if ((aux = barcos_posicionados - config.frag) >= 0)
    {
        if ((aux -= config.dest) >= 0)
        {
            if ((aux -= config.acor) >= 0)
            {
                if ((aux -= config.subm) < 0)
                    aux += config.subm;
                //else aux queda igual
            } else
                aux += config.acor;
        } else
            aux += config.dest;
    } else
        aux += config.frag;

    ship_n = aux;

    if (place_ship(PLAYER_1, size, ship_n, aux_orient, aux_i, aux_j) < 0)
    {
        change_status(1);
        return;
    } else
    {
        change_status(0);
    }

    ++barcos_posicionados;
    update_board_images();

    if (barcos_posicionados - config.frag < 0)  //tamanho del barco actual
        gtk_label_set_text(ship_label, "Fragata");
    else if (barcos_posicionados - config.frag - config.dest < 0)
        gtk_label_set_text(ship_label, "Destructor");
    else if (barcos_posicionados - config.frag - config.dest - config.acor < 0)
        gtk_label_set_text(ship_label, "Acorazado");
    else if (barcos_posicionados - config.frag - config.dest  - config.acor - config.subm < 0)
        gtk_label_set_text(ship_label, "Submarino");
    else if (barcos_posicionados - config.tot < 0)
        gtk_label_set_text(ship_label, "Portaviones");
    else
        gtk_label_set_text(ship_label, "");

    if (barcos_posicionados == config.tot)
    {
        change_status(2);
        gtk_widget_hide(orient_button);     //hide config buttons
        gtk_widget_hide(place_ship_ok);
        gtk_widget_hide(new_game_button);
        gtk_widget_show(start_button);      //show start button
    }
}

int shoot_cpu_gui()
{
    /* Dispara como PLAYER_2 */

    //codigo adaptado de shoot_rand() y gameloop()
    int res = 0;
    int fila, col;
    int sunk;

    if (gamemode == 1)
        return FALSE;

    if (turn == PLAYER_2 && fase == 1)
    {
        //se cuentan los barcos vivos, se realiza el disparo, se recuentan los barcos
        if (difficulty == 0)    //random
        {
            do
            {
                fila = gen_num(0, 9);
                col = gen_num(0, 9);

                res = validateShot(PLAYER_2, fila, col);
            } while (res < 0);
            shoot(PLAYER_2, fila, col);
        } else if (difficulty == 1) //hunt/target
        {
            res = HTshoot(&fila, &col);
        } else if (difficulty == 2) //probability density
        {
            res = PDshoot(&fila, &col);
        }

        tot_p2_shots++;

        //se combrueba si se hunde un barco
        sunk = mark_sunk(PLAYER_1);
        if (sunk != -1)
        {
            lives_1--;
            if (difficulty == 2)
                mark_sunk_PD(sunk);
        }

        //se actualiza el tablero grafico y el estado del juego
        update_board_images();
        update_ships_images();

        if (res == 1)
        {
            tot_p2_hits++;
            change_status(7);
        }
        else if (res == 0)
        {
            change_status(3);
            turn = PLAYER_1;
        }

        //se revisan condiciones de fin del juego
        if (lives_1 == 0)
        {
            fase = 2;
            run_game_over();
        } else if (limite_disparos == turnos && config.start == PLAYER_1 && turn == PLAYER_1)
        {
            fase = 2;
            run_game_over();
        }
    }

    return TRUE;    //para que el g_timeout siga llamando la funcion
}

void board_cb(GtkWidget *event_box, GdkEventButton *event, gpointer data)
{
    /*  Funcion de callback para un evento button-press-event en un tablero.
        Contiene las funciones de posicionamiento de barcos y de disparos para el jugador.
    */

    int res;
    int sunk;

    if (gamemode == 1)  //modo pc vs pc
        return;

    //coordenadas
    guint i, j;
    i = (GUINT_FROM_LE(event->y) / 32); //dimensiones de imagenes 32x32
    j = (GUINT_FROM_LE(event->x) / 32);

    if (fase == 0)  //colocacion de barcos
    {
        if (data == board_op)
        {
            return;
        } else if (data == board_ju)
        {
            place_ship_gui(i, j);
            return;
        }
    } else if (fase == 1)   //disparos
    {
        if (turn != PLAYER_1)   //solo puede registrar disparos en el turno y tablero correcto
            return;
        else if (data == board_ju)
        {
            change_status(8);
            return;
        }

        res = validateShot(PLAYER_1, i, j);

        if (res == -2)
        {
            change_status(5);
            return;         //disparo no valido, no causa cambios
        } else if (res == 1)
        {
            tot_p1_hits++;
            change_status(6);
        } else if (res == 0)
        {
            change_status(4);
            turn = PLAYER_2;
            turnos++;
        }

        shoot(PLAYER_1, i, j);
        tot_p1_shots++;

        //se comprueba si se hundio un barco
        sunk = mark_sunk(PLAYER_2);
        if (sunk != -1)
        {
            lives_2--;
        }

        update_board_images();
        update_ships_images();

        //Fin del juego
        if (lives_2 == 0)
        {
            fase = 2;
            run_game_over();
        } else if (turnos == limite_disparos && config.start == PLAYER_2 && turn == PLAYER_2)
        {
            fase = 2;
            run_game_over();
        }
    }
}

GtkWidget * create_board_ju()
{
    /*  Crea el tablero del jugador usando un eventbox e imagenes de fondo.
          Retorno:
            el tablero
    */

    GtkWidget *eventbox;
    GtkWidget *image;   //auxiliar para cargar imagen

    eventbox = gtk_event_box_new();
    board_ju = gtk_grid_new();

    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            image = gtk_image_new_from_file(img[0]);
            gtk_grid_attach(GTK_GRID(board_ju), image, i, j, 1, 1);
            images_ju[j][i] = GTK_IMAGE(image); //se almacenan los widgets de imagenes para modificarse mas tarde
        }
    }

    gtk_container_add(GTK_CONTAINER(eventbox), board_ju);
    g_signal_connect(eventbox, "button-press-event", G_CALLBACK(board_cb), board_ju);
    return eventbox;
}

GtkWidget * create_board_op()
{
    /*  Crea el tablero del oponente usando un eventbox e imagenes de fondo.
          Retorno:
            - el tablero
    */

    GtkWidget *eventbox;
    GtkWidget *image;   //auxiliar para cargar imagen

    eventbox = gtk_event_box_new();
    board_op = gtk_grid_new();

    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            image = gtk_image_new_from_file(img[0]);
            gtk_grid_attach(GTK_GRID(board_op), image, i, j, 1, 1);
            images_op[j][i] = GTK_IMAGE(image); //se almacenan los widgets de imagenes para modificarse mas tarde
        }
    }

    gtk_container_add(GTK_CONTAINER(eventbox), board_op);
    g_signal_connect(eventbox, "button-press-event", G_CALLBACK(board_cb), board_op);

    return eventbox;
}

GtkWidget * create_row()
{
    /* Crea las columnas que indican la fila del tablero
        Retorno:
            La columna
    */

    GtkImage *image;
    GtkWidget *row;

    row = gtk_grid_new();

    for (int i = 0; i < FILAS; i++)
    {
        image = gtk_image_new_from_file(img[17+i]);
        gtk_grid_attach(row, image, 0, i, 1, 1);
    }

    return row;
}

GtkWidget * create_col()
{
    /* Crea las filas que indican la columna del tablero
        Retorno:
            La fila
    */

    GtkImage *image;
    GtkWidget *col;

    col = gtk_grid_new();

    for (int i = 0; i < FILAS; i++)
    {
        image = gtk_image_new_from_file(img[17+i]);
        gtk_grid_attach(col, image, i, 0, 1, 1);
    }

    return col;
}

/* Funciones de juego */
void new_game()
{
    /* Inicializa los tableros y la configuracion del juego como para iniciar un juego nuevo */

    extern int HTmode;
    extern int PDactive;

    if (HTmode != -1)
    {
        HTmode = -1;    //cambiar estado de la bandera
    }
    else if (PDactive == 1)
    {
        PDactive = 0;   //cambiar estado de la bandera
    }

    for (int i = 0; i < config.tot; i++)
    {
        if (fleet_1 != NULL)
        {
            if (fleet_1[i].pos != NULL)
            {
                free(fleet_1[i].pos);
                fleet_1[i].pos = NULL;
            }
        }
        if (fleet_2 != NULL)
        {
            if (fleet_2[i].pos != NULL)
            {
                free(fleet_2[i].pos);
                fleet_2[i].pos = NULL;
            }
        }
    }

    reset();
    fase = 0;
    barcos_posicionados = 0;
    turnos = 0;
    aux_i = -10;
    aux_j = -10;

    tot_p1_shots = 0;
    tot_p1_hits = 0;
    tot_p2_shots = 0;
    tot_p2_hits = 0;

    run_config();

    if (gamemode == 0)
    {
        gtk_widget_show(place_ship_config);
        gtk_widget_show(orient_button);     //show config buttons
        gtk_widget_show(place_ship_ok);
        gtk_widget_hide(new_game_button);
        gtk_widget_hide(start_button);      //hide start button
        change_status(0);
    } else
    {
        gtk_widget_show(place_ship_config);
        gtk_widget_hide(orient_button);     //hide config buttons
        gtk_widget_hide(place_ship_ok);
        gtk_widget_hide(new_game_button);
        gtk_widget_show(start_button);      //show start button
    }

    gtk_widget_hide(p_boats_label);     //hide fleet status
    gtk_widget_hide(c_boats_label);
    gtk_widget_hide(p_boats_grid);
    gtk_widget_hide(c_boats_grid);

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            gtk_image_set_from_file(images_ju[i][j], img[0]);
            gtk_image_set_from_file(images_op[i][j], img[0]);
        }
    }

    if (gamemode == 1)  //el programa ya habra colocado sus barcos
    {
        reset_mystery();
        update_board_images();
        change_status(2);
        gtk_widget_hide(GTK_LABEL(ship_label));
        gtk_widget_show(start_button);
    }
}

void confirm_restart(GtkWidget *widget, gpointer data)
{
    /* Si el juego esta en progreso, pregunta por una confirmacion antes de reiniciar el juego.
       Se llama al usar la opcion de menu "Nuevo".
    */

    GtkDialog *restart_dialog;

    if (fase == 0 || fase == 1)
    {
        restart_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                    GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_QUESTION,
                    GTK_BUTTONS_OK_CANCEL, "Una partida ya esta en progreso\nReiniciar?");

        if (gtk_dialog_run(GTK_DIALOG(restart_dialog)) == GTK_RESPONSE_OK)
			new_game();

        gtk_widget_destroy(GTK_WIDGET(restart_dialog));
    } else
    {
        new_game();
    }
}

void start_game()
{
    /* El juego progresa a la fase de disparos. Desbloquea funciones de disparos y bloquea funciones
       de configuracion.
    */

    extern int PDactive;

    if (gamemode == 0)
        if (barcos_posicionados < config.tot)
            return;

    if (gamemode == 1)
    {
        init_PD();
        PDactive = 0;   //causa que mark_sunk() no marque posiciones para PDA en caso se llame

        if (turn == PLAYER_1)   //limpia historial.txt
            pvp_reset_historial();

        //funcion turno a ser llamada periodicamente
        g_timeout_add_seconds(1, G_SOURCE_FUNC(pvp_turn), NULL);
    } else if (difficulty == 0)
    {
        setup_board_rand();
        g_timeout_add_seconds(1, G_SOURCE_FUNC(shoot_cpu_gui), NULL);
    } else if (difficulty == 1)
    {
        init_HT();
        setup_board_rand();
        g_timeout_add_seconds(1, G_SOURCE_FUNC(shoot_cpu_gui), NULL);
    } else if (difficulty == 2)
    {
        init_PD();
        setup_board_PD();
        g_timeout_add_seconds(1, G_SOURCE_FUNC(shoot_cpu_gui), NULL);
    }

    fase = 1;

    gtk_widget_hide(place_ship_config);

    lives_1 = config.tot;   //contadores de barcos vivos
    lives_2 = config.tot;

    update_ships_images();

    if (gamemode == 0)
    {
        if (turn == PLAYER_1)
            change_status(3);
        else
            change_status(4);
    } else
        change_status(9);
}

void end_game()
{
    /* Termina el juego y muestra el mensaje de fin de juego */

    int ganador;

    char game_over_0[22];   //los nombres tienen un limite de 15 chars
    char game_over_1[22];   //el numero 22 o 39 viene de sumarle los caracteres del mensaje en si
    char game_over_5[39];
    char game_over_6[39];

    snprintf(game_over_0, 22, game_over[0], p_name);
    snprintf(game_over_1, 22, game_over[1], c_name);
    snprintf(game_over_5, 39, game_over[5], p_name);
    snprintf(game_over_6, 39, game_over[6], c_name);

    ganador = winner();

    if (ganador == -1)  //empate
    {
        if (lives_1 == config.tot)  //no se hundieron barcos
        {
            gtk_label_set_text(game_over_victory, game_over[2]);
            gtk_label_set_text(game_over_text, game_over[8]);
        } else
        {
            gtk_label_set_text(game_over_victory, game_over[2]);
            gtk_label_set_text(game_over_text, game_over[7]);
        }
    }
    if (turnos < config.shot)  //flota hundida
    {
        if (ganador == PLAYER_1)
        {
            gtk_label_set_text(game_over_victory, game_over_0);
            gtk_label_set_text(game_over_text, game_over[3]);
        } else if (ganador == PLAYER_2)
        {
            gtk_label_set_text(game_over_victory, game_over_1);
            gtk_label_set_text(game_over_text, game_over[4]);
        }
    } else if (limite_disparos == config.shot)  //limite alcanzado
    {
        if (ganador == PLAYER_1)
        {
            gtk_label_set_text(game_over_victory, game_over_0);
            gtk_label_set_text(game_over_text, game_over_5);
        } else if (ganador == PLAYER_2)
        {
            gtk_label_set_text(game_over_victory, game_over_1);
            gtk_label_set_text(game_over_text, game_over_6);
        }
    }
}
