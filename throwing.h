/** @file   move.h
    @author Gordon Lay and Henry Mossman
    @date   17/10/2019
    @brief  A helper module for player movement

    @defgroup move A helper module for player movement

    This module uses tinygl_point to 'move' the player pixel when
    certain navswitch inputs are read
*/

#ifndef THROWING_H
#define THROWING_H

#include "system.h"

//allows player movement and ability to throw the ball to the
//other funkit
int move(int playerNumber);

#endif
