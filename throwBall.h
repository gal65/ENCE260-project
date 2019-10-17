/** @file   throwBall.h
    @author Gordon Lay and Henry Mossman
    @date   17/10/2019
    @brief  A helper module for throwing the ball

    @defgroup move A helper module for throwing the ball

    This module uses an LED mat to make it look as if a pixel is moving
    across the led matrix
*/

#ifndef throwBall_H
#define throwBall_H

#include "system.h"

//simulates a ball being thrown
void throw (uint8_t playerPosition);

#endif
