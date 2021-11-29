#ifndef PCVSPC_H_
#define PCVSPC_H_

/*  Contiene funciones necesarias para el modo de juego pc vs pc
*/

///* Libera bloques de memoria dinamica */
//void pvp_free();

/* Guarda el path al directorio en el que se juega */
void pvp_set_path(gchar *folder);

/* Coloca los barcos del programa */
void pvp_setup_board();

/* Reinicializa mystery_bpard */
void reset_mystery();

/* Llama funciones segun turn; genera disparos o respuestas.
    Retorno:
        TRUE: si el juego continua
        FALSE: si el juego termina
*/
int pvp_turn();

#endif // PCVSPC_H_
