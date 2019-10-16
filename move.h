/** @file   mov.h
    @author Gordon Lay and Henry Mossman
    @date   17/10/2019
    @brief  A helper module for player movement

    @defgroup move A helper module for player movement

    This module uses tinygl_point to 'move' the player pixel when
    certain navswitch inputs are read
*/

#ifndef MOVE_H
#define MOVE_H

#include "system.h"

//Move right
void moveRight(uint8_t count);

//Move left
void moveLeft(uint8_t count);

//Move in general
int move(void);

#endif
