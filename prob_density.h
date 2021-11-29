#ifndef PROB_DENSITY_H_
#define PROB_DENSITY_H_

/*  Header for the Probability Density Algorithm (PDA).
*/

#define HIT_WEIGHT 25
#define POSSIBILITY_AMNT 20

struct PDcell
{
    int x, y;       //coordinates
    int state;      //0 if unknown, HIT, MISS, or SUNK if shot
    int density;    //number of possible ways the cell can be used
};

struct PDship
{
    int size, len;  //size 0-4, len 1-4
    int sunk;       //0 if false, 1 if true
};

//Functions

/* Initializes PDboard and player_fleet */
void init_PD();

/* Prints the densities of each cell to visualize it */
void print_density_grid();

/* Resets all cells' densities to 0 */
void reset_density();

/* Tests to see if a ship can be placed in a position and orientation. Counts density. */
void PD_place_ship(int ship_size, int x, int y, int orient);

/* Calculates probable locations for every boat that is still not sunk */
void calculate_density();

/* Shoots a location likely to contain a ship. If there are multiple, shoots at one decided randomly */
int PDshoot(int *i, int *j);

/* Marks a ship as sunk for the PDA
    size: 0-4 (type)
*/
void mark_sunk_PD(int size);

#endif // PROB_DENSITY_H_
