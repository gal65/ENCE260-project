/** @file   game.c
    @author Gordon Lay and Henry Mossman
    @date   17/10/2019
    @brief  A simple ball catching game

    @defgroup game A simple ball catching game.
*/

#include <string.h>
#include "system.h"
#include "tinygl.h"
#include "task.h"
#include "navswitch.h"
#include "eeprom.h"
#include "uint8toa.h"
#include "../fonts/font3x5_1.h"
#include "pacer.h"
#include <avr/io.h>
#include "tweeter.h"
#include "mmelody.h"
#include "button.h"
#include "pio.h"
#include <stdbool.h>
#include "ir_serial.h"
#include "ir.h"
#include "move.h"

#define PACER_RATE 500
#define MESSAGE_RATE 25
#define ready_char 'z'

//Ready pending boolean
static _Bool ready;

//Initialise the ready boolean to false
static void ready_screen_init(void)
{
    ready = false;
}

//Task: ready is now true
static void ready_screen_task(void)
{
    ready = true;
}

//Initialising tinygl font, speed, scroll and rotate settings
static void tinygl_task_init(void)
{
    tinygl_font_set(&font3x5_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
}

//IR error message
static void show_err (uint8_t err)
{
    char buffer[3];
    buffer[0] = 'E';
    buffer[1] = err + '0';
    buffer[2] = 0;
    tinygl_text (buffer);
}

//Ready-state loop; shows message 'READY?' while sending and checking
//for 1's transmitted via serial ir
static void ready_loop (void)
{
    uint16_t errorTime = 0;
    tinygl_clear();
    tinygl_text("READY?");
    //Signal to other funkit that this funkit is ready
    ir_serial_transmit (ready_char);
    while (1)
    {
        uint8_t data;
        pacer_wait();
        tinygl_update();
        //Receive serial ir signals
        ir_serial_ret_t ret;
        ret = ir_serial_receive (&data);
        //If the ir signal is ok and it is the ready character,
        //return to main
        if (ret == IR_SERIAL_OK) {
            if (data == ready_char) {
                ir_serial_transmit (ready_char);
                return;
            }
            //else diplay error code for some time, then return to the
            //top of main
        } else if (ret < 0) {
            tinygl_clear();
            show_err (-ret);
            while(1) {
                pacer_wait();
                tinygl_update();
                errorTime++;
                if (errorTime >= 1800) {
                    main();
                }
            }
        }
    }
}

//The game function
static void game (void)
{
    //Start with loading the move function
    move();
}


int main (void)
{
    //Initiation
    ready_screen_init();
    button_init();
    system_init();
    ir_serial_init();
    pacer_init (PACER_RATE);
    tinygl_init(PACER_RATE);
    tinygl_task_init();
    //Intro screen
    tinygl_text("CATCHIN BALL Z");

    while(1)
    {
        pacer_wait();
        tinygl_update();
        button_update();
        //Break out of Intro screen and into the Ready loop
        if (button_push_event_p(BUTTON1) && !ready) {
            ready_screen_task();
            ready_loop();
            //When both players are ready, start the game
            tinygl_clear();
            uint16_t startTime = 0;
            tinygl_text("GAME START!");
            game();
        }
    }
    return 0;
}

