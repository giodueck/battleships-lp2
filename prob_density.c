#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "gameloop.h"
#include "prob_density.h"

struct PDcell PDboard[10][10];  //Grid with board information visible to the algorithm

extern int fleetsize[5];
extern int fleetsize_n;
extern int fleetb_2[10][10];

int PDactive = 0;       //tells the rest of the program whether the PDA was activated
int PD_fleetsize[5];    //counts live ships by type

void init_PD()
{
    /* Initializes PDboard */

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            PDboard[i][j].x = i;
            PDboard[i][j].y = j;
            PDboard[i][j].state = 0;
            PDboard[i][j].density = 0;
        }
    }

    PDactive = 1;

    for (int i = 0; i < 5; i++)     //counts live ships
    {
        PD_fleetsize[i] = fleetsize[i];
    }
}

void setup_board_PD()
{
    /* Places PLAYER_2's ships with certain rules, generally randomly */

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
                        if (place_ship(PLAYER_2, size, n, orient, x, y) == 0)
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
                            if (x+1 <= 9 && fleetb_2[x+1][y+j] != 0) //must not touch a ship to either side
                            {
                                error = 1;
                                break;
                            } else if (x-1 >= 0 && fleetb_2[x-1][y+j] != 0)
                            {
                                error = 1;
                                break;
                            } else if (y-1 >= 0 && fleetb_2[x][y-1] != 0)   //must not touch either end with a ship
                            {
                                error = 1;
                                break;
                            } else if (y+len <= 9 && fleetb_2[x][y+len] != 0)
                            {
                                error = 1;
                                break;
                            }
                        }
                    } else if (orient == VERTICAL)
                    {
                        for (int i = 0; i < len; i++)
                        {
                            if (y+1 <= 9 && fleetb_2[x+i][y+1] != 0) //must not touch a ship to either side
                            {
                                error = 1;
                                break;
                            } else if (y-1 >= 0 && fleetb_2[x+i][y-1] != 0)
                            {
                                error = 1;
                                break;
                            } else if (x-1 >= 0 && fleetb_2[x-1][y] != 0)   //must not touch either end with a ship
                            {
                                error = 1;
                                break;
                            } else if (x+len <= 9 && fleetb_2[x+len][y] != 0)
                            {
                                error = 1;
                                break;
                            }
                        }
                    }
                }

                if (error == 0)
                    error = place_ship(PLAYER_2, size, n, orient, x, y);
            } while (error);
        }
    }
}

void print_density_grid()
{
    /* Prints the densities of each cell to visualize it */
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            printf(" %d\t|", PDboard[i][j].density);
            if (j == 9)
                printf("\n");
        }
    }
}

void reset_density()
{
    /* Resets all cells' densities to 0 */

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            PDboard[i][j].density = 0;
        }
    }
}

void PD_place_ship(int ship_size, int x, int y, int orient)
{
    /* Tests to see if a ship can be placed in a position and orientation. Counts density. */

    int len;
    int error = 0;
    int hits = 0;   //if a hit is marked in the position the ship is placed, density will be inflated

    if (ship_size < 3)
        len = ship_size + 1;
    else
        len = ship_size;

    if (orient == HORIZONTAL)
    {
        for (int j = 0; j < len; j++)   //tests all cells along the length of the ship
        {
            if (y+j > 9)
            {
                error = 1;
                break;
            } else if (PDboard[x][y+j].state == MISS || PDboard[x][y+j].state == SUNK)
            {
                error = 1;
                break;
            } else if (PDboard[x][y+j].state == HIT)
            {
                hits++;
            }
        }
    } else if (orient == VERTICAL)
    {
        for (int i = 0; i < len; i++)
        {
            if (x+i > 9)
            {
                error = 1;
                break;
            } else if (PDboard[x+i][y].state == MISS || PDboard[x+i][y].state == SUNK)
            {
                error = 1;
                break;
            } else if (PDboard[x+i][y].state == HIT)
            {
                hits++;
            }
        }
    }

    if (!error)
    {
        if (orient == HORIZONTAL)
        {
            for (int j = 0; j < len; j++)
            {
                //if (PDboard[x][x+j].state == 0) //if shot, a cell has no probability of having another ship
                    PDboard[x][y+j].density += (1 + hits*HIT_WEIGHT);
            }
        } else if (orient == VERTICAL)
        {
            for (int i = 0; i < len; i++)
            {
                //if (PDboard[x+i][y].state == 0)
                    PDboard[x+i][y].density += (1 + hits*HIT_WEIGHT);
            }
        }
    }
}

void sim_placements(int ship_size)
{
    /* Simulates all possible ship placements for a given ship_size. Considers the state of the cells and,
       if possible to place, increments the density counters for all cells used every time they were used.
       The result is a superposition of all possible locations for a cell.
    */

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            PD_place_ship(ship_size, i, j, 0);
            if (ship_size > 0)
                PD_place_ship(ship_size, i, j, 1);
        }
    }

    for (int i = 0; i < 10; i++)    //if already shot set density to 0. Shouldn't be needed, just in case
    {
        for (int j = 0; j < 10; j++)
        {
            if (PDboard[i][j].state != 0)
                PDboard[i][j].density = 0;
        }
    }
}

void calculate_density()
{
    /* Calculates probable locations for every boat that is still not sunk */

    reset_density();

    for (int ship_size = 0; ship_size < 5; ship_size++)
    {
        for (int j = 0; j < PD_fleetsize[ship_size]; j++)
        {
            sim_placements(ship_size);
        }
    }
}

int PDshoot(int *i, int *j)
{
    /* Shoots a location likely to contain a ship. If there are multiple, shoots at one decided randomly */

    int max_density;
    struct PDcell possibilities[POSSIBILITY_AMNT];    //cells with the highest probability will be stored for one to be shot
    int ctr = 0;
    int shoot_at;   //takes a value between 0 and ctr-1
    int hit;

    calculate_density();

    for (int k = 0; k < 10; k++)
    {
        for (int l = 0; l < 10; l++)
        {
            if (k == 0 && l == 0)                           //first maximum
                max_density = PDboard[k][l].density;
            else if (PDboard[k][l].density > max_density)    //update max_density
                max_density = PDboard[k][l].density;
        }
    }

    for (int k = 0; k < 10 && ctr < POSSIBILITY_AMNT-1; k++)
    {
        for (int l = 0; l < 10; l++)
        {
            if (ctr == POSSIBILITY_AMNT-1)   //only 20 cells will be stored
            {
                break;
            } else if (PDboard[k][l].density == max_density)   //store cell
            {
                possibilities[ctr] = PDboard[k][l];
                ctr++;
            }
        }
    }

    //generate number between 0 and ctr-1 and shoot at that cell
    shoot_at = rand() % ctr;
    *i = possibilities[shoot_at].x;
    *j = possibilities[shoot_at].y;
    hit = validateShot(PLAYER_2, *i, *j);
    shoot(PLAYER_2, *i, *j);

    if (hit)
        PDboard[*i][*j].state = HIT;
    else
        PDboard[*i][*j].state = MISS;

    return hit;
}

void mark_sunk_PD(int size)
{
    /* Marks a ship as sunk for the PDA */

    PD_fleetsize[size]--;
}
