#ifndef HUNT_TARGET_H_
#define HUNT_TARGET_H_

/*  Header for the Hunt/Target Algorithm HTA.
*/

//Modes
#define HUNT    0
#define TARGET  1

//Stack
#define QUEUE_SIZE 100

struct cell
{
    int x, y;   //coordinates
    int state;  //0 if not shot, HIT or MISS if shot
};

//Functions

/* Initialize HTboard and allocate space to potential queue */
void init_HT();

/* Shoots a cell depending on HTmode */
int HTshoot(int *i, int *j);

#endif // HUNT_TARGET_H_
