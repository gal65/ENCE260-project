#include "system.h"
#include "pacer.h"
#include "pio.h"
#include "navswitch.h"
#include "ir_serial.h"


//Define pace in Hz
#define PACER_RATE 1000

#define TICK_NUMBER 60


//Define PIO pins and LED Mat rows and columns
static pio_t ledmat_rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, LEDMAT_ROW4_PIO,
    LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO, LEDMAT_ROW7_PIO
};

static pio_t ledmat_cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


static void ledmat_pixel_set (int col, int row, bool state)
{
    if (state)
    {
        pio_output_low (ledmat_rows[row]);
        pio_output_low (ledmat_cols[col]);
    }
    else
    {
        pio_output_high (ledmat_rows[row]);
        pio_output_high (ledmat_cols[col]);
    }
}

//initialise led matrix
static void ledmat_init (void)
{
    uint8_t row;
    uint8_t col;

    //initialise each row
    for (row = 0; row < 7; row++)
    {
        pio_config_set (ledmat_rows[row], PIO_OUTPUT_HIGH);
        pio_output_high (ledmat_rows[row]);
    }

    //initialise each column
    for (col = 0; col < 5; col++)
    {
        pio_config_set (ledmat_cols[col], PIO_OUTPUT_HIGH);
        pio_output_high (ledmat_cols[col]);
    }
}

//simulates a ball being thrown
void throw (uint8_t playerPosition) {
    pacer_init(PACER_RATE);
    ledmat_init();
    ir_serial_init();
    uint8_t row = playerPosition;
    int col = 4;
    ledmat_pixel_set (4, playerPosition, 1);
    uint64_t tick = 0;
    while (1)
    {
        pacer_wait ();
        ledmat_pixel_set (col, row, 0);
        ledmat_pixel_set (4, playerPosition, 1);
        //decrement the column
        col--;
        while(1) {
            pacer_wait();
            ledmat_pixel_set (col, row, 1);
            tick++;
            //keep the led on for some time using ticks
            if (tick >= TICK_NUMBER) {
                tick = 0;
                break;
            }
        }
        //in the case that col = 0, hold it for some time using ticks
        //then turn it off and return to main
        if (col == 0) {
            while(1)
            {
                pacer_wait();
                ledmat_pixel_set (col, row, 1);
                tick++;
                if (tick >= TICK_NUMBER) {
                    tick = 0;
                    break;
                }
            }
            ledmat_pixel_set (col, row, 0);
            navswitch_update();
            return;
        }
    }
}

