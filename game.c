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
#include "throwing.h"
#include "led.h"
#include "throwBall.h"
#include "catchBall.h"

#define PACER_RATE 500
#define MESSAGE_RATE 25
#define ready_char 'z'
#define receive_char 'r'
#define throwing_char 't'

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

//Ready-state loop; shows message 'READY?' while sending and checking
//for signals transmitted via serial ir, then determines who starts
//and puts the players in catch/moves loops for the duration of the
//game
int ready_loop (void)
{
    tinygl_clear();
    tinygl_text("READY?");
    int playerNumber = 2;
    while(1)
    {
        pacer_wait();
        tinygl_update();
        navswitch_update();
        uint8_t data;
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            ir_serial_transmit (receive_char);
        }
        //Receive serial ir signals
        ir_serial_ret_t ret;
        ret = ir_serial_receive (&data);
        if (ret == IR_SERIAL_OK) {
            //If the ir signal is ok and it is the receiving character,
            //this funkit is player 0
            if (data == receive_char) {
                playerNumber = 7;
                ir_serial_transmit (throwing_char);
                while(1) {
                    move(catching(playerNumber));
                }
            //else if the throwing char is received,
            //this funkit is player 1
            } else if (data == throwing_char) {
                playerNumber = 8;
                while(1) {
                    catching(move(playerNumber));
                }
            }
        //else diplay error code for some time, then return to the
        //top of main
        } else if (ret < 0) {
            tinygl_clear();
            while(1) {
                pacer_wait();
                tinygl_update();
                return -1;
            }
        }
    }
}

int main (void)
{
    //Initiation
    ready_screen_init();
    button_init();
    system_init();
    ir_serial_init();
    pacer_init (PACER_RATE);
    tinygl_init (PACER_RATE);
    tinygl_task_init();
    //Intro messaage
    tinygl_text("CATCHIN BALL Z");

    while(1)
    {
        pacer_wait();
        tinygl_update();
        button_update();
        if (button_push_event_p (BUTTON1)) {
            tinygl_text("CATCHIN BALL Z");
            while ((ready_loop() == -1) && !ready)
            {
                //Break out of Intro screen and into the Ready loop
                ready_screen_task();
                ready_loop();
                //When both players are ready, start the game
            }
        }
    }
    return 0;
}

