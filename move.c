/** @file   move.c
    @author Gordon Lay and Henry Mossman
    @date   17/10/2019
    @brief  Helper module for player movement

    @defgroup move Helper module for player movement
*/

#include "tinygl.h"
#include "navswitch.h"
#include "uint8toa.h"
#include "pacer.h"
#include <avr/io.h>
#include "system.h"

#define PACER_RATE_MOVE 1000

void moveRight(uint8_t count)
{
    /*count will be your current position. X is always 4. moving right moves the player towards the zero y
     * index. this means count must have 1 taken off in when it moves right*/
    tinygl_coord_t x = 4;
    tinygl_coord_t y1 = count - 1;
    tinygl_coord_t y = count;
    tinygl_point_t pointPrev = {x, y};
    /* the previous point must be turned off using y = count*/
    tinygl_point_t pointNew = {x, y1};

    /*Setting new*/
    pacer_wait();
    tinygl_pixel_set (pointPrev, 0); // off
    tinygl_pixel_set (pointNew, 1); // on
    tinygl_update();
}

/*funtion to move play left*/
void moveLeft(uint8_t count)
{
    tinygl_coord_t x = 4;
    tinygl_coord_t y1 = count + 1;
    tinygl_coord_t y = count;
    tinygl_point_t pointPrev = {x, y};
    tinygl_point_t pointNew = {x, y1};

    /*Setting new*/
    pacer_wait();
    tinygl_pixel_set (pointPrev, 0);
    tinygl_pixel_set (pointNew, 1);
    tinygl_update();
}

int move(void)
{
    system_init ();
    tinygl_init (PACER_RATE_MOVE);
    pacer_init (PACER_RATE_MOVE);
    uint8_t count = 3;
    tinygl_coord_t x = 4;
    tinygl_coord_t y = 3; // player starting point
    tinygl_point_t pointS = {x, y};
    tinygl_pixel_set (pointS, 1);
    /* change y value to move along */

    while (1) {
        pacer_wait ();
        navswitch_update ();
        tinygl_update ();
        /* points then tingly update */

        if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
            if (count >= 1) {
                moveRight(count);
            /*special case, player is stuck corner, teleport to other*/
            } else if (count == 0) {
                tinygl_coord_t y = count;
                tinygl_coord_t x = 4;
                tinygl_point_t pointPrev = {x, y};
                pacer_wait();
                tinygl_pixel_set (pointPrev, 0);
                tinygl_update();

                moveLeft(5);
                count = 7;
            }
            count --;
        }

        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            if (count < 6) {
                moveLeft(count);
            /*special case, player is stuck corner, teleport to other*/
            } else if (count == 6) {
                tinygl_coord_t y = count;
                tinygl_coord_t x = 4;
                tinygl_point_t pointPrev = {x, y};
                pacer_wait();
                tinygl_pixel_set (pointPrev, 0);
                tinygl_update();

                moveRight(1);
                count = -1;
            }
            count ++;
        }
    }
    //Return count for future use
    return count;
}
