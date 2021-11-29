/*
    BATALLA NAVAL
    TPF
    Autor: Giovanni Dueck
    Version: 1.0.1
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gtk/gtk.h>
#include "gameloop.h"
#include "rand.h"
#include "board.h"
#include "gtk_callbacks.h"

GtkWidget *window;  //ventana principal y componentes
GObject *board_box_ju;  //contiene al tablero del jugador
GObject *board_box_op;  //contiene al tablero del oponente
GtkLabel *status_label;
GtkWidget *board_ju;    //tableros
GtkLabel *p_board_label;
GtkWidget *board_op;
GtkLabel *c_board_label;
GtkWidget *p_row;       //indices
GtkWidget *p_col;
GtkWidget *c_row;
GtkWidget *c_col;
GObject *barcos_ju;
GObject *barcos_op;
GtkImage *images_ju[10][10];   //guarda las imagenes del tablero
GtkImage *images_op[10][10];   //guarda las imagenes del tablero

GtkWidget *place_ship_config;
GtkLabel *ship_label;
GtkButton *orient_button;
GtkButton *place_ship_ok;
GtkButton *new_game_button;
GtkButton *start_button;

GtkGrid *p_boats_grid;  //visualizacion de flotas
GtkGrid *c_boats_grid;
GtkLabel *p_boats_label;
GtkLabel *c_boats_label;
GtkLabel *p_boats_tot;
GtkLabel *p_boats_tot_n;
GtkLabel *p_boats_frag;
GtkLabel *p_boats_dest;
GtkLabel *p_boats_acor;
GtkLabel *p_boats_subm;
GtkLabel *p_boats_port;
GtkLabel *c_boats_tot;
GtkLabel *c_boats_tot_n;
GtkLabel *c_boats_frag;
GtkLabel *c_boats_dest;
GtkLabel *c_boats_acor;
GtkLabel *c_boats_subm;
GtkLabel *c_boats_port;
GtkLabel *p_boats_turns;
GtkLabel *p_boats_turns1;
GtkLabel *p_boats_turns_n;
GtkImage *p_boats_images[20];
GtkImage *c_boats_images[20];

GObject *menu_juego_nuevo;  //items de menu
GObject *menu_juego_estadisticas;
GObject *menu_juego_salir;
GObject *menu_ayuda_instrucciones;
GObject *menu_ayuda_acerca_de;

GtkDialog *config_dialog;   //dialogo de configuracion y componentes
GtkButton *config_reset;  //botones del cuadro de dialogo
GtkButton *config_ok;
GtkLabel *config_warning;
GtkLabel *gamemode_sel;  //gamemode
GtkLabel *gamemode_label;
GtkButton *gamemode_left;
GtkButton *gamemode_right;
GtkLabel *diff_select;  //difficulty
GtkLabel *diff_label;
GtkButton *diff_left;
GtkButton *diff_right;
GtkLabel *frag_sel;      //frag
GtkLabel *frag_label;
GtkButton *frag_left;
GtkButton *frag_right;
GtkLabel *dest_sel;      //dest
GtkLabel *dest_label;
GtkButton *dest_left;
GtkButton *dest_right;
GtkLabel *acor_sel;      //acor
GtkLabel *acor_label;
GtkButton *acor_left;
GtkButton *acor_right;
GtkLabel *subm_sel;      //subm
GtkLabel *subm_label;
GtkButton *subm_left;
GtkButton *subm_right;
GtkLabel *port_sel;      //port
GtkLabel *port_label;
GtkButton *port_left;
GtkButton *port_right;
GtkLabel *shot_sel;      //shot
GtkLabel *shot_label;
GtkEntry *shot_entry;
GtkLabel *start_sel;     //start
GtkLabel *start_label;
GtkButton *start_left;
GtkButton *start_right;
GtkLabel *name_ju;      //nombres
GtkLabel *name_op;
GtkEntry *name_ju_entry;
GtkEntry *name_op_entry;

GtkDialog *instruc_dialog;  //dialogo de configuracion y componentes
GtkButton *instruc_ok;
GtkLabel *instruc_text;         //texto

GtkDialog *game_over_dialog;    //dialogo de fin de juego
GtkLabel *game_over_victory;
GtkLabel *game_over_text;
GtkButton *game_over_new;
GtkButton *game_over_quit;
GtkLabel *p1_name;          //estadisticas fin de juego
GtkLabel *p2_name;
GtkLabel *p1_sunk;
GtkLabel *p2_sunk;
GtkLabel *p1_shots;
GtkLabel *p2_shots;
GtkLabel *p1_hits;
GtkLabel *p2_hits;
GtkLabel *p1_accuracy;
GtkLabel *p2_accuracy;

GtkDialog *about_dialog;        //dialogo acerca de
GtkImage *about_logo;

GtkDialog *stats_dialog;        //dialogo estadisticas
GtkButton *stats_close;
GtkLabel *stats_msg;    //por si no hay estadisticas
GtkGrid *stats_grid;

GtkDialog *path_dialog;     //dialogo para seleccion de directorio modo pvp
GtkButton *path_ok;
GtkFileChooserButton *path_file_chooser;
GtkLabel *path_msg;

extern char *img[];
extern struct stats_row *row;

extern int fleetsize_n;
extern struct ship *fleet_1;
extern struct ship *fleet_2;

/* Indica la fase del juego. Funciones que modifican el estado del juego se revisan
   este valor para decidir si ejecutan la funcion o no
*/
int fase = -1;   //0->colocacion de barcos, 1->disparos, 2->finalizado

int tot_p1_shots;   //estadisticas de partida
int tot_p1_hits;
int tot_p2_shots;
int tot_p2_hits;

int main(int argc, char *argv[])
{
    //Builder
    GtkBuilder *builder;
    GError *error = NULL;
    guint ret;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    ret = gtk_builder_add_from_file(builder, "tpf_batalla_naval.glade", &error);
    if (ret == 0)
    {
		g_print("Error en la funcion gtk_builder_add_from_file:\n%s", error->message);
		return 1;
	}
	g_free(error);

    // Construccion de la interfaz
    //Ventana principal
    window = GTK_WIDGET(gtk_builder_get_object(builder, "principal"));
    gtk_window_set_title(GTK_WINDOW(window), "Batalla Naval");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "delete-event", G_CALLBACK(confirm_quit_delete), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    board_box_ju = gtk_builder_get_object(builder, "board_box_ju");
    board_box_op = gtk_builder_get_object(builder, "board_box_op");
    p_board_label = GTK_LABEL(gtk_builder_get_object(builder, "p_board_label"));
    c_board_label = GTK_LABEL(gtk_builder_get_object(builder, "c_board_label"));
    p_row = gtk_builder_get_object(builder, "p_row");
    p_col = gtk_builder_get_object(builder, "p_col");
    c_row = gtk_builder_get_object(builder, "c_row");
    c_col = gtk_builder_get_object(builder, "c_col");

    barcos_ju = gtk_builder_get_object(builder, "barcos_ju");
    barcos_op = gtk_builder_get_object(builder, "barcos_op");

    status_label = GTK_LABEL(gtk_builder_get_object(builder, "status_label"));

    place_ship_config = GTK_WIDGET(gtk_builder_get_object(builder, "place_ship_config"));
    ship_label = GTK_LABEL(gtk_builder_get_object(builder, "ship_label"));
    orient_button = GTK_BUTTON(gtk_builder_get_object(builder, "orient_button"));
    g_signal_connect(orient_button, "clicked", G_CALLBACK(orient_button_cb), NULL);
    place_ship_ok = GTK_BUTTON(gtk_builder_get_object(builder, "place_ship_ok"));
    g_signal_connect(place_ship_ok, "clicked", G_CALLBACK(place_ship_ok_cb), NULL);
    new_game_button = GTK_BUTTON(gtk_builder_get_object(builder, "new_game_button"));
    g_signal_connect(new_game_button, "clicked", G_CALLBACK(new_game), NULL);
    start_button = GTK_BUTTON(gtk_builder_get_object(builder, "start_button"));
    g_signal_connect(start_button, "clicked", G_CALLBACK(start_game), NULL);

    p_boats_grid = GTK_GRID(gtk_builder_get_object(builder, "p_boats_grid"));
    c_boats_grid = GTK_GRID(gtk_builder_get_object(builder, "c_boats_grid"));
    p_boats_label = GTK_LABEL(gtk_builder_get_object(builder, "p_boats_label"));
    c_boats_label = GTK_LABEL(gtk_builder_get_object(builder, "c_boats_label"));
    p_boats_tot = GTK_LABEL(gtk_builder_get_object(builder, "p_boats_tot"));
    p_boats_tot_n = GTK_LABEL(gtk_builder_get_object(builder, "p_boats_tot_n"));
    p_boats_frag = GTK_LABEL(gtk_builder_get_object(builder, "p_boats_frag"));
    p_boats_dest = GTK_LABEL(gtk_builder_get_object(builder, "p_boats_dest"));
    p_boats_acor = GTK_LABEL(gtk_builder_get_object(builder, "p_boats_acor"));
    p_boats_subm = GTK_LABEL(gtk_builder_get_object(builder, "p_boats_subm"));
    p_boats_port = GTK_LABEL(gtk_builder_get_object(builder, "p_boats_port"));
    c_boats_tot = GTK_LABEL(gtk_builder_get_object(builder, "c_boats_tot"));
    c_boats_tot_n = GTK_LABEL(gtk_builder_get_object(builder, "c_boats_tot_n"));
    c_boats_frag = GTK_LABEL(gtk_builder_get_object(builder, "c_boats_frag"));
    c_boats_dest = GTK_LABEL(gtk_builder_get_object(builder, "c_boats_dest"));
    c_boats_acor = GTK_LABEL(gtk_builder_get_object(builder, "c_boats_acor"));
    c_boats_subm = GTK_LABEL(gtk_builder_get_object(builder, "c_boats_subm"));
    c_boats_port = GTK_LABEL(gtk_builder_get_object(builder, "c_boats_port"));
    p_boats_turns = GTK_LABEL(gtk_builder_get_object(builder, "p_boats_turns"));
    p_boats_turns1 = GTK_LABEL(gtk_builder_get_object(builder, "p_boats_turns1"));
    p_boats_turns_n = GTK_LABEL(gtk_builder_get_object(builder, "p_boats_turns_n"));

    gtk_box_pack_start(GTK_BOX(p_row), create_row(), TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(c_row), create_row(), TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(p_col), create_col(), TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(c_col), create_col(), TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(board_box_ju), create_board_ju(), TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(board_box_op), create_board_op(), TRUE, FALSE, 0);

    //Dialogos
      //Configuracion de partida
    config_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "config_dialog"));
    gtk_window_set_title(GTK_WINDOW(config_dialog), "Configuracion de partida");
    gtk_window_set_resizable(GTK_WINDOW(config_dialog), FALSE);
    gtk_window_set_transient_for(GTK_WINDOW(config_dialog), GTK_WINDOW(window));
    gtk_window_set_position(GTK_WINDOW(config_dialog), GTK_WIN_POS_CENTER_ON_PARENT);

        //Botones y labels
    gamemode_sel = GTK_LABEL(gtk_builder_get_object(builder, "gamemode_select"));
    gamemode_label = GTK_LABEL(gtk_builder_get_object(builder, "gamemode_label"));
    gamemode_left = GTK_BUTTON(gtk_builder_get_object(builder, "gamemode_left"));
    gamemode_right = GTK_BUTTON(gtk_builder_get_object(builder, "gamemode_right"));
    g_signal_connect(gamemode_left, "clicked", G_CALLBACK(gamemode_cb), NULL);
    g_signal_connect(gamemode_right, "clicked", G_CALLBACK(gamemode_cb), NULL);

    diff_select = GTK_LABEL(gtk_builder_get_object(builder, "diff_select"));
    diff_label = GTK_LABEL(gtk_builder_get_object(builder, "diff_label"));
    diff_left = GTK_BUTTON(gtk_builder_get_object(builder, "diff_left"));
    diff_right = GTK_BUTTON(gtk_builder_get_object(builder, "diff_right"));
    g_signal_connect(diff_left, "clicked", G_CALLBACK(difficulty_cb), diff_left);
    g_signal_connect(diff_right, "clicked", G_CALLBACK(difficulty_cb), diff_right);

    frag_sel = GTK_LABEL(gtk_builder_get_object(builder, "frag_select"));
    frag_label = GTK_LABEL(gtk_builder_get_object(builder, "frag_label"));
    frag_left = GTK_BUTTON(gtk_builder_get_object(builder, "frag_left"));
    g_signal_connect(frag_left, "clicked", G_CALLBACK(ship_sub), (gpointer) FRAG);
    frag_right = GTK_BUTTON(gtk_builder_get_object(builder, "frag_right"));
    g_signal_connect(frag_right, "clicked", G_CALLBACK(ship_add), (gpointer) FRAG);

    dest_sel = GTK_LABEL(gtk_builder_get_object(builder, "dest_select"));
    dest_label = GTK_LABEL(gtk_builder_get_object(builder, "dest_label"));
    dest_left = GTK_BUTTON(gtk_builder_get_object(builder, "dest_left"));
    g_signal_connect(dest_left, "clicked", G_CALLBACK(ship_sub), (gpointer) DEST);
    dest_right = GTK_BUTTON(gtk_builder_get_object(builder, "dest_right"));
    g_signal_connect(dest_right, "clicked", G_CALLBACK(ship_add), (gpointer) DEST);

    acor_sel = GTK_LABEL(gtk_builder_get_object(builder, "acor_select"));
    acor_label = GTK_LABEL(gtk_builder_get_object(builder, "acor_label"));
    acor_left = GTK_BUTTON(gtk_builder_get_object(builder, "acor_left"));
    g_signal_connect(acor_left, "clicked", G_CALLBACK(ship_sub), (gpointer) ACOR);
    acor_right = GTK_BUTTON(gtk_builder_get_object(builder, "acor_right"));
    g_signal_connect(acor_right, "clicked", G_CALLBACK(ship_add), (gpointer) ACOR);

    subm_sel = GTK_LABEL(gtk_builder_get_object(builder, "subm_select"));
    subm_label = GTK_LABEL(gtk_builder_get_object(builder, "subm_label"));
    subm_left = GTK_BUTTON(gtk_builder_get_object(builder, "subm_left"));
    g_signal_connect(subm_left, "clicked", G_CALLBACK(ship_sub), (gpointer) SUBM);
    subm_right = GTK_BUTTON(gtk_builder_get_object(builder, "subm_right"));
    g_signal_connect(subm_right, "clicked", G_CALLBACK(ship_add), (gpointer) SUBM);

    port_sel = GTK_LABEL(gtk_builder_get_object(builder, "port_select"));
    port_label = GTK_LABEL(gtk_builder_get_object(builder, "port_label"));
    port_left = GTK_BUTTON(gtk_builder_get_object(builder, "port_left"));
    g_signal_connect(port_left, "clicked", G_CALLBACK(ship_sub), (gpointer) PORT);
    port_right = GTK_BUTTON(gtk_builder_get_object(builder, "port_right"));
    g_signal_connect(port_right, "clicked", G_CALLBACK(ship_add), (gpointer) PORT);

    shot_sel = GTK_LABEL(gtk_builder_get_object(builder, "shot_select"));
    shot_label = GTK_LABEL(gtk_builder_get_object(builder, "shot_label"));
    shot_entry = GTK_ENTRY(gtk_builder_get_object(builder, "shot_entry"));
    g_signal_connect(shot_entry, "changed", G_CALLBACK(shot_cb), NULL);

    start_sel = GTK_LABEL(gtk_builder_get_object(builder, "start_select"));
    start_label = GTK_LABEL(gtk_builder_get_object(builder, "start_label"));
    start_left = GTK_BUTTON(gtk_builder_get_object(builder, "start_left"));
    start_right = GTK_BUTTON(gtk_builder_get_object(builder, "start_right"));
    g_signal_connect(start_left, "clicked", G_CALLBACK(start_cb), start_left);
    g_signal_connect(start_right, "clicked", G_CALLBACK(start_cb), start_right);

    name_ju = GTK_LABEL(gtk_builder_get_object(builder, "name_ju"));
    name_op = GTK_LABEL(gtk_builder_get_object(builder, "name_op"));
    name_ju_entry = GTK_ENTRY(gtk_builder_get_object(builder, "name_ju_entry"));
    name_op_entry = GTK_ENTRY(gtk_builder_get_object(builder, "name_op_entry"));

    config_warning = GTK_LABEL(gtk_builder_get_object(builder, "config_warning"));

    config_reset = GTK_BUTTON(gtk_builder_get_object(builder, "config_reset"));
    g_signal_connect(config_reset, "clicked", G_CALLBACK(config_reset_cb), NULL);

    config_ok = GTK_BUTTON(gtk_builder_get_object(builder, "config_ok"));
    g_signal_connect(config_ok, "clicked", G_CALLBACK(config_ok_cb), NULL);

      //Intrucciones
    instruc_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "instruc_dialog"));
    gtk_window_set_title(GTK_WINDOW(instruc_dialog), "Ayuda para Batalla Naval");
    gtk_window_set_resizable(GTK_WINDOW(instruc_dialog), FALSE);
    gtk_window_set_default_size(GTK_WINDOW(instruc_dialog), 400, 300);
    gtk_window_set_transient_for(GTK_WINDOW(instruc_dialog), GTK_WINDOW(window));
    gtk_window_set_position(GTK_WINDOW(instruc_dialog), GTK_WIN_POS_CENTER_ON_PARENT);

        //Botones y labels
    instruc_ok = GTK_BUTTON(gtk_builder_get_object(builder, "instruc_ok"));
    g_signal_connect(instruc_ok, "clicked", G_CALLBACK(instruc_ok_cb), NULL);
    instruc_text = GTK_LABEL(gtk_builder_get_object(builder, "instruc_text"));

      //Fin del juego
    game_over_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "game_over_dialog"));
    gtk_window_set_title(GTK_WINDOW(game_over_dialog), "Fin del juego");
    gtk_window_set_transient_for(GTK_WINDOW(game_over_dialog), GTK_WINDOW(window));
    gtk_window_set_position(GTK_WINDOW(game_over_dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    game_over_victory = GTK_LABEL(gtk_builder_get_object(builder, "game_over_victory"));
    game_over_text = GTK_LABEL(gtk_builder_get_object(builder, "game_over_text"));
    game_over_new = GTK_BUTTON(gtk_builder_get_object(builder, "game_over_new"));
    g_signal_connect(game_over_new, "clicked", G_CALLBACK(game_over_new_cb), NULL);
    game_over_quit = GTK_BUTTON(gtk_builder_get_object(builder, "game_over_quit"));
    g_signal_connect(game_over_quit, "clicked", G_CALLBACK(game_over_quit_cb), NULL);

        //Estadisticas
    p1_name = GTK_LABEL(gtk_builder_get_object(builder, "p1_name"));
    p2_name = GTK_LABEL(gtk_builder_get_object(builder, "p2_name"));
    p1_sunk = GTK_LABEL(gtk_builder_get_object(builder, "p1_sunk"));
    p2_sunk = GTK_LABEL(gtk_builder_get_object(builder, "p2_sunk"));
    p1_shots = GTK_LABEL(gtk_builder_get_object(builder, "p1_shots"));
    p2_shots = GTK_LABEL(gtk_builder_get_object(builder, "p2_shots"));
    p1_hits = GTK_LABEL(gtk_builder_get_object(builder, "p1_hits"));
    p2_hits = GTK_LABEL(gtk_builder_get_object(builder, "p2_hits"));
    p1_accuracy = GTK_LABEL(gtk_builder_get_object(builder, "p1_accuracy"));
    p2_accuracy = GTK_LABEL(gtk_builder_get_object(builder, "p2_accuracy"));

      //Acerca de
    about_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "about_dialog"));
    about_logo = GTK_IMAGE(gtk_image_new_from_file(img[16]));
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(about_dialog), gtk_image_get_pixbuf(about_logo));
    gtk_window_set_transient_for(GTK_WINDOW(about_dialog), GTK_WINDOW(window));
    gtk_window_set_position(GTK_WINDOW(about_dialog), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(about_dialog), "Acerca de Batalla Naval");

      //Estadisticas
    stats_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "stats_dialog"));
    gtk_window_set_transient_for(GTK_WINDOW(stats_dialog), GTK_WINDOW(window));
    gtk_window_set_position(GTK_WINDOW(stats_dialog), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(stats_dialog), "Estadisticas");

        //Botones y labels
    stats_grid = GTK_GRID(gtk_builder_get_object(builder, "stats_grid"));
    stats_msg = GTK_LABEL(gtk_builder_get_object(builder, "stats_msg"));

    stats_close = GTK_BUTTON(gtk_builder_get_object(builder, "stats_close"));
    g_signal_connect(stats_close, "clicked", G_CALLBACK(stats_close_cb), NULL);

      //Seleccion de diretorio (pvp)
    path_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "path_dialog"));
    gtk_window_set_title(GTK_WINDOW(path_dialog), "Path");
    gtk_window_set_transient_for(GTK_WINDOW(path_dialog), GTK_WINDOW(config_dialog));
    gtk_window_set_position(GTK_WINDOW(path_dialog), GTK_WIN_POS_CENTER);
    path_file_chooser = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "path_file_shooser"));
    path_msg = GTK_LABEL(gtk_builder_get_object(builder, "path_msg"));
    path_ok = GTK_BUTTON(gtk_builder_get_object(builder, "path_ok"));
    g_signal_connect(path_ok, "clicked", G_CALLBACK(path_ok_cb), NULL);

    //Items de menus
    menu_juego_nuevo = gtk_builder_get_object(builder, "juego_nuevo");
    g_signal_connect(menu_juego_nuevo, "activate", G_CALLBACK(confirm_restart), NULL);

    menu_juego_estadisticas = gtk_builder_get_object(builder, "juego_estadisticas");
    g_signal_connect(menu_juego_estadisticas, "activate", G_CALLBACK(run_stats), NULL);

    menu_juego_salir = gtk_builder_get_object(builder, "juego_salir");
    g_signal_connect(menu_juego_salir, "activate", G_CALLBACK(confirm_quit), NULL);

    menu_ayuda_instrucciones = gtk_builder_get_object(builder, "ayuda_instrucciones");
    g_signal_connect(menu_ayuda_instrucciones, "activate", G_CALLBACK(run_instruc), NULL);

    menu_ayuda_acerca_de = gtk_builder_get_object(builder, "ayuda_acerda_de");
    g_signal_connect(menu_ayuda_acerca_de, "activate", G_CALLBACK(run_about), NULL);

    //Visibilidad
    gtk_widget_show_all(window);

    //Start
    srand(time(0));
    init_config();
    set_ships_images();

    gtk_main();

    //free allocated memory
    if (row != NULL)
        free(row);
    for (int i = 0; i < fleetsize_n; i++)
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

    return 0;
}
