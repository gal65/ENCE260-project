#include "system.h"
#include "pacer.h"
#include "pio.h"
#include "navswitch.h"
#include "ir_serial.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "display.h"

//Define pace in Hz
#define PACER_RATE 3000
//define number of ticks
#define TICK_NUMBER 450

#define MESSAGE_RATE 25

//Initialising tinygl font, speed, scroll and rotate settings
static void tinygl_task_init(void)
{
    tinygl_font_set(&font3x5_1);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
}

void moveRightRec(uint8_t count)
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
void moveLeftRec(uint8_t count)
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

//Define PIO pins and LED Mat rows and columns
static pio_t ledmat_rows[] = {
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, LEDMAT_ROW4_PIO,
    LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO, LEDMAT_ROW7_PIO
};
static pio_t ledmat_cols[] = {
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};

static void ledmat_pixel_set (int col, int row, bool state)
{
    if (state) {
        pio_output_low (ledmat_rows[row]);
        pio_output_low (ledmat_cols[col]);
    } else {
        pio_output_high (ledmat_rows[row]);
        pio_output_high (ledmat_cols[col]);
    }
}

static void ledmat_init (void)
{
    uint8_t row;
    uint8_t col;
    //initialise each row
    for (row = 0; row < 7; row++) {
        pio_config_set (ledmat_rows[row], PIO_OUTPUT_HIGH);
        pio_output_high (ledmat_rows[row]);
    }

    //initialise each column
    for (col = 0; col < 5; col++) {
        pio_config_set (ledmat_cols[col], PIO_OUTPUT_HIGH);
        pio_output_high (ledmat_cols[col]);
    }
}

//Reads ball row from ir and sends it down the column
//also allows player to move side to side
int catch (uint8_t ballRow, uint8_t playerPosition)
{
    ir_serial_init();
    pacer_init(PACER_RATE);
    ledmat_init();
    tinygl_init(PACER_RATE);
    uint8_t row = ballRow;
    int col = 0;
    ledmat_pixel_set (4, playerPosition, 1);
    uint64_t tick = 0;
    while (1) {
        pacer_wait ();
        ledmat_pixel_set (4, playerPosition, 1);
        if (row != playerPosition) {
            for (int i = 0; i < 4; i++) {
                ledmat_pixel_set (i, playerPosition, 0);
                ledmat_pixel_set (i, (6-playerPosition), 0);
                ledmat_pixel_set (4, (playerPosition), 0);
            }
        }
        ledmat_pixel_set (col, row, 0);
        col++;
        while(1) {
            pacer_wait();
            if (row != playerPosition) {
                for (int i = 0; i < 4; i++) {
                    ledmat_pixel_set (i, playerPosition, 0);
                }
            }
            ledmat_pixel_set (col, row, 1);
            tick++;
            //keep the led on for some time using ticks
            if (tick >= TICK_NUMBER) {
                if (row != playerPosition) {
                    ledmat_pixel_set (4, playerPosition, 1);
                    for (int i = 0; i < 4; i++) {
                        ledmat_pixel_set (i, (6-playerPosition), 0);
                        ledmat_pixel_set (4, (playerPosition), 0);
                    }
                }
                tinygl_clear();
                tick = 0;
                break;
            }
        }
        //in the case that col = 0, hold it for some time using ticks
        //then turn it off and return to main
        if (col == 4) {
            while(1) {
                pacer_wait();
                ledmat_pixel_set (col, row, 1);
                tick++;
                if (tick >= TICK_NUMBER) {
                    ledmat_pixel_set (4, playerPosition, 1);
                    if (row != playerPosition) {
                        for (int i = 0; i < 4; i++) {
                            ledmat_pixel_set (i, (6-playerPosition), 0);
                            ledmat_pixel_set (4, (playerPosition), 0);
                        }
                    }
                    tick = 0;
                    break;
                }
            }
            tinygl_clear();
            if (row != playerPosition) {
                tinygl_text("YOU LOSE :(");
                while(1) {
                    pacer_wait();
                    tinygl_update();
                }
            }
            ledmat_pixel_set (col, row, 0);
            navswitch_update();
            return 8;
        }
    }
}

int catching(int playerNumber)
{
    system_init();
    tinygl_init (PACER_RATE);
    int state = playerNumber - 7;
    pacer_init(PACER_RATE);
    tinygl_task_init();
    ir_serial_init();
    uint8_t count = 3;
    uint8_t data;
    tinygl_coord_t x = 4;
    tinygl_coord_t y = 3; // player starting point
    tinygl_point_t pointS = {x, y};
    tinygl_pixel_set (pointS, 1);
    /* change y value to move along */
    while(1) {
        pacer_wait();
        while (1) {
            pacer_wait ();
            navswitch_update ();
            tinygl_update ();
            /* points then tingly update */

            if (navswitch_push_event_p (NAVSWITCH_NORTH))
            {
                if (count >= 1) {
                    moveRightRec(count);
                /*special case, player is stuck corner, teleport to other*/
                } else if (count == 0) {
                    tinygl_coord_t y = count;
                    tinygl_coord_t x = 4;
                    tinygl_point_t pointPrev = {x, y};
                    pacer_wait();
                    tinygl_pixel_set (pointPrev, 0);
                    tinygl_update();

                    moveLeftRec(5);
                    count = 7;
                }
                count --;
            }

            if (navswitch_push_event_p (NAVSWITCH_SOUTH))
            {
                if (count < 6) {
                    moveLeftRec(count);
                /*special case, player is stuck corner, teleport to other*/
                } else if (count == 6) {
                    tinygl_coord_t y = count;
                    tinygl_coord_t x = 4;
                    tinygl_point_t pointPrev = {x, y};
                    pacer_wait();
                    tinygl_pixel_set (pointPrev, 0);
                    tinygl_update();

                    moveRightRec(1);
                    count = -1;
                }
                count ++;
            }
            if (state == 0) {
                ir_serial_ret_t ret;
                ret = ir_serial_receive (&data);
                if ((ret == IR_SERIAL_OK) && (data <= 6)) {
                    return catch((6-data), count);
                }
            }
        }
    }
}


