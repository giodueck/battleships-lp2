#include <stdlib.h>
#include "board.h"
#include "rand.h"
#include "gameloop.h"
#include "hunt_target.h"

int HTmode = -1;  //HUNT o TARGET

struct cell HTboard[10][10];    //board with information about each cell. The algorithm will only have access to this board
struct cell *potential[QUEUE_SIZE];     //queue of cells to shoot at. Pointer to pointer to HTboard
int pot_top;                    //points to first free position
int pot_bottom;                 //points to the first occupied position, or pot_top if empty
int pot_size = 0;               //size of queue

void init_HT()
{
    /* Initialize HTboard */

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            HTboard[i][j].state = 0;
            HTboard[i][j].x = i;
            HTboard[i][j].y = j;
        }
    }
    pot_top = 0;
    pot_bottom = 0;
    pot_size = QUEUE_SIZE;
    HTmode = HUNT;
}

void pot_write(int x, int y)
{
    /* Adds a cell to the potential queue */
    potential[pot_top++] = &HTboard[x][y];
}

struct cell* pot_read()
{
    /* Returns the next cell in queue */
    struct cell *next;
    next = potential[pot_bottom++];
    return next;
}

void mark_cell(int x, int y, int state)
{
    /* Changes a cells state component */
    HTboard[x][y].state = state;
}

int hunt_shoot(int *i, int *j)
{
    /* Shoots randomly. On hit adds cells to stack and changes HTmode */

    int hit = 0;
    int x, y;

    do  //random shot
    {
        x = gen_num(0, 9);
        y = gen_num(0, 9);

        hit = validateShot(PLAYER_2, x, y);
    } while (hit < 0);

    *i = x;
    *j = y;

    shoot(PLAYER_2, x, y);

    if (hit == 1)   //hit
    {
        HTmode = TARGET;
        mark_cell(x, y, HIT);

        //if an adjacent cell has not yet been shot, add to queue
        if (validateShot(PLAYER_2, x+1, y) >= 0)
            pot_write(x+1, y);

        if (validateShot(PLAYER_2, x-1, y) >= 0)
            pot_write(x-1, y);

        if (validateShot(PLAYER_2, x, y+1) >= 0)
            pot_write(x, y+1);

        if (validateShot(PLAYER_2, x, y-1) >= 0)
            pot_write(x, y-1);

    } else      //miss
    {
        mark_cell(x, y, MISS);
    }

    return hit;
}

int target_shoot(int *i, int *j)
{
    /* Shoots a potential cell from the queue. If the queue is empty, try hunt_shoot() */

    int x, y;
    int hit;
    int res;
    struct cell *target;

    if (pot_top > pot_bottom)   //if the queue isn't empty
    {
        target = pot_read();
        x = target->x;
        y = target->y;
        *i = x;
        *j = y;

        //if hit, add cells to queue, if not, do nothing
        if ((hit = validateShot(PLAYER_2, x, y)) == 1)
        {
            //add surrounding cells to queue
            res = hit;
            shoot(PLAYER_2, x, y);
            mark_cell(x, y, HIT);


            //if an adjacent cell has not yet been shot, add to queue
            if (validateShot(PLAYER_2, x+1, y) >= 0)
                pot_write(x+1, y);

            if (validateShot(PLAYER_2, x-1, y) >= 0)
                pot_write(x-1, y);

            if (validateShot(PLAYER_2, x, y+1) >= 0)
                pot_write(x, y+1);

            if (validateShot(PLAYER_2, x, y-1) >= 0)
                pot_write(x, y-1);
        } else if (hit == 0)
        {
            res = hit;
            shoot(PLAYER_2, x, y);
        }

        if (hit < 0) //if not valid, try next in queue
        {
            res = target_shoot(i, j);
        }
    } else
    {
        HTmode = HUNT;
        res = hunt_shoot(i, j);
    }

    return res;
}

int HTshoot(int *i, int *j)
{
    /* Shoots a cell depending on HTmode */

    int res;

    if (HTmode == HUNT)
    {
        res = hunt_shoot(i, j);
    } else if (HTmode == TARGET)
    {
        res = target_shoot(i, j);
    }

    return res;
}
