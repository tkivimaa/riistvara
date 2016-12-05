#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <time.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "cli_microrl.h"
#include "print_helper.h"
#include "hmi_msg.h"
#include "uart-wrapper.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/helius_microrl/microrl.h"
#include "../lib/hd44780_111/hd44780.h"

#define BLINK_DELAY_MS 100
#define BAUD 9600
#define UART_STATUS_MASK 0x00FF
#define LED_START DDRA |= _BV(DDA3)
#define LED_SWITCH PORTA ^= _BV(PORTA3)


//microrl with object and pointer
static microrl_t rl;
static microrl_t *prl = &rl;

static inline void displayName(void);
static inline void initialize(void);
static inline void initializeCli(void);
static inline void initSystemClock(void);
static inline void heartbeat(void);
int main (void)
{
    displayName(); //Display my name on LED and console
    print_ver(stderr); //Display versions in stderr
    initialize(); //initialize workspace
    initializeCli(); //Initialize CLI for use

    while (1) {
        heartbeat();
        // CLI commands are handled in cli_execute()
        microrl_insert_char (prl, cli_get_char());
    }
}
static inline void initializeCli(void)
{
    // Call init with ptr to microrl instance and print callback
    microrl_init (prl, cli_print);
    // Set callback for execute
    microrl_set_execute_callback (prl, cli_execute);
}
static inline void initialize(void)
{
    /* set pin 3 of PORTA for output*/
    LED_START;
    initSystemClock();
    /* Init error console as stderr in UART3 and print user code info */
    uart3_init(UART_BAUD_SELECT(BAUD, F_CPU)); //Init uart3
    uart0_init(UART_BAUD_SELECT(BAUD, F_CPU)); //init uart0
    sei(); //for interrupts
    stdin = stdout = &uart0_io;
    stderr = &uart3_out;
    lcd_init();
    lcd_clrscr();
}

static inline void displayName(void)
{
    lcd_puts_P(studName); //Student name to LCD
    fprintf_P(stdout, studName); //Student name to console
    fprintf(stdout, "\n"); //new line after student name, to console
}
static inline void heartbeat(void)
{
    static time_t prev_time; //previous time to compare with current time
    time_t cur_time = time(NULL);

    if (cur_time <= prev_time) { //If 1 second has passed
        return;
    }

    prev_time = cur_time; //set times to match to start counting new time
    fprintf_P(stderr, uptime, cur_time); //Print uptime to
    fprintf(stderr, "\n"); //line break after each line.
    LED_SWITCH;
}
static inline void initSystemClock(void)
{
    TCCR5A = 0; // Clear control register A
    TCCR5B = 0; // Clear control register B
    TCCR5B |= _BV(WGM52) | _BV(CS52); // CTC and fCPU/256
    OCR5A = 62549; // 1 s
    TIMSK5 |= _BV(OCIE5A); // Output Compare A Match Interrupt Enable
}
/* Time */
ISR(TIMER5_COMPA_vect)
{
    system_tick();
}