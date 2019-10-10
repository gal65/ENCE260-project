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
#include "pio.h"

#define PACER_RATE 500
#define MESSAGE_RATE 15
enum {TUNE_BPM_RATE = 200};
enum {TUNE_TASK_RATE = 100};
enum {TWEETER_TASK_RATE = 5000};

/* Connect piezo tweeter to eight and sixth pin of UCFK4 P1 connector
   for push-pull operation.  For single-ended drive (with reduced
   volume) connect the other piezo connection to ground or Vcc and do
   not define PIEZO2_PIO.  */
#define PIEZO1_PIO PIO_DEFINE (PORT_D, 1)
#define PIEZO2_PIO PIO_DEFINE (PORT_D, 3)

static tweeter_scale_t scale_table[] = TWEETER_SCALE_TABLE (TWEETER_TASK_RATE);
static tweeter_t tweeter;
static mmelody_t melody;
static mmelody_obj_t melody_info;
static tweeter_obj_t tweeter_info;

static const char game_intro_tune[] =
{
#include "intro_music.mmel"
"> "
};

static void tweeter_task_init (void)
{
    tweeter = tweeter_init (&tweeter_info, TWEETER_TASK_RATE, scale_table);

    pio_config_set (PIEZO1_PIO, PIO_OUTPUT_LOW);
#ifdef PIEZO2_PIO
    pio_config_set (PIEZO2_PIO, PIO_OUTPUT_LOW);
#endif
}


static void tweeter_task (__unused__ void *data)
{
    bool state;

    state = tweeter_update (tweeter);

    pio_output_set (PIEZO1_PIO, state);
#ifdef PIEZO2_PIO

    pio_output_set (PIEZO2_PIO, !state);
#endif
}


static void tune_task_init (void)
{
    melody = mmelody_init (&melody_info, TUNE_TASK_RATE,
               (mmelody_callback_t) tweeter_note_play, tweeter);

    mmelody_speed_set (melody, TUNE_BPM_RATE);
}

static void tune_task (__unused__ void *data)
{
    mmelody_update (melody);
}

static void tinygl_task_init(void)
{
    tinygl_init(PACER_RATE);
    tinygl_font_set(&font3x5_1);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    tinygl_text_speed_set(MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
}

int main (void)
{
    task_t tasks[] =
    {
        {.func = tweeter_task, .period = TASK_RATE / TWEETER_TASK_RATE,
         .data = 0},
        {.func = tune_task, .period = TASK_RATE / TUNE_TASK_RATE,
         .data = 0},
    };

    system_init();
    pacer_init (PACER_RATE);
    tinygl_task_init();
    tweeter_task_init();
    tune_task_init();
    tinygl_text("SUPER DODGEBALL 9000");

    while(1)
    {
        pacer_wait();
        tinygl_update();
        mmelody_update (melody);
    }
    task_schedule (tasks, ARRAY_SIZE (tasks));
    mmelody_play (melody, game_intro_tune);
    return 0;

}

