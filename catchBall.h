/** @file   catchBall.h
    @author Gordon Lay and Henry Mossman
    @date   17/10/2019
    @brief  A helper module for sending the ball across ir

    @defgroup move A helper module for sending the ball across ir

    This module uses an LED mat to make it look as if a pixel is moving
    across the led matrix
*/

#ifndef catchBall_H
#define catchBall_H

#include "system.h"

//Reads ball row from ir and sends it down the column
int catching(int playerNumber);

#endif
