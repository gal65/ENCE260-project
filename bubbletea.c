#include "system.h"
#include "pacer.h"
#include "display.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 4


int main (void)
{
    int moveIn;
    int col;
    int colinc;
    int dist;

    system_init ();
    display_init ();
    dist = 4; // count the number of rows travled.

    moveIn = 3; // change to input form move.
    col= 4; // start for ball throw.
    display_pixel_set (col, moveIn, 1); //initial position

    pacer_init (LOOP_RATE);
    while (1)
    {
        pacer_wait ();
        if (dist >= 0) {

            display_pixel_set (col, moveIn, 0); // turn off pixel.

            col = dist; // postion of ball at next step.

            display_pixel_set (col, moveIn, 1); // turn on next.


        }
        dist --;
        display_update ();
    }
}





