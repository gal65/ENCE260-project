#include <string.h>
#include "system.h"
#include "tinygl.h"
#include "task.h"
//#include "navswitch.h"
//#include "flasher.h"
//#include "eeprom.h"
#include "uint8toa.h"
#include "../fonts/font3x5_1.h"
//#include "tweeter.h"
//#include "mmelody.h"
#include "pio.h"
#include "pacer.h"
#include <avr/io.h>

#define PACER_RATE 500
#define MESSAGE_RATE 15

int main (void)
{
    system_init();

    tinygl_init(PACER_RATE);
    tinygl_font_set(&font3x5_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);

    tinygl_text("Super Dodgeball 9000");


    pacer_init (PACER_RATE);

    while(1)
    {
        pacer_wait();
        tinygl_update();

    }
    return 0;
}

