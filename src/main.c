#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "../lib/andygock_avr-uart/uart.h"
#include "print_helper.h"
#include "hmi_msg.h"
#include "uart-wrapper.h"
#include "../lib/hd44780_111/hd44780.h"

#define BLINK_DELAY_MS 100
#define BAUD 9600

/* Global milliseconds counter*/
volatile uint32_t seconds;

static inline void displayMonth(void);
static inline void displayName(void);
static inline void displayVersion(void);
static inline void displayAscii(void);
static inline void initialize(void);
static inline void initSystemClock(void);
static inline void heartbeat(void);
int main (void)
{
    initialize(); //initialize workspace
    displayName(); //Display my name on LED and console
    displayVersion(); //Display versions in console
    displayAscii(); //Display ascII table in console in 2 ways

    while (1) {
        heartbeat();

        if (uart0_available()) {
            displayMonth(); //ask for month + display
        }
    }
}
static inline void initialize(void)
{
    /* set pin 3 of PORTA for output*/
    DDRA |= _BV(DDA3);
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
static inline void displayAscii(void)
{
    print_ascii_tbl(stdout);
    unsigned char ascii[128] = {0};

    for (unsigned char i = 0; i < 128; i++) {
        ascii[i] = i;
    }

    print_for_human(stdout, ascii, 128);
    fprintf_P(stdout, monthList); //Ask for month for displayMonth
}
static inline void displayVersion(void)
{
    fprintf_P(stderr, PSTR(VER_FW), PSTR(GIT_DESCR), PSTR(__DATE__),
              PSTR(__TIME__));
    fprintf_P(stderr, PSTR(VER_LIBC), PSTR(__AVR_LIBC_VERSION_STRING__));
}
static inline void displayName(void)
{
    lcd_puts_P(studName); //Student name to LCD
    fprintf_P(stdout, studName); //Student name to console
    fprintf(stdout, "\n"); //new line after student name, to console
}
static inline void displayMonth(void)
{
    char month;
    fscanf(stdin, "%c", &month); //User enters a month first letter
    fprintf(stdout, "%c\n", month); //print selected letter to console
    lcd_clr(LCD_ROW_2_START, 16); //Clear row 2
    lcd_goto(LCD_ROW_2_START); //Go to begin of row 2

    for (int i = 0; i < 6; i++) {
        if (!strncmp_P(&month, (PGM_P)pgm_read_word(&nameMonth[i]),
                       1)) { //Find month that matches character month
            fprintf_P(stdout, (PGM_P)pgm_read_word(
                          &nameMonth[i])); //Put found month to console
            lcd_puts_P((PGM_P)pgm_read_word(&nameMonth[i])); //Put found month to LCD
            lcd_putc(' '); //For LCD we print space to separate 2 months
            fprintf(stdout, "\n"); //New line after a month is found
        }
    }

    fprintf_P(stdout, monthList); //Asks the next month first letter.
}
static inline void heartbeat(void)
{
    static uint32_t prev_seconds; //previous time to compare with current time
    uint32_t cur_seconds = seconds; //Current time copy to memory

    if ((prev_seconds - cur_seconds) > 0) { //If 1 second has passed
        PORTA ^= _BV(PORTA3); //Toggle portA to inverse state
        fprintf_P(stderr, uptime, cur_seconds); //Print uptime to
    }

    prev_seconds =
        cur_seconds; //Set previous time to start counting the next second from
}
static inline void initSystemClock(void)
{
    TCCR5A = 0; // Clear control register A
    TCCR5B = 0; // Clear control register B
    TCCR5B |= _BV(WGM52) | _BV(CS52); // CTC and fCPU/256
    OCR5A = 62549; // 1 s
    TIMSK5 |= _BV(OCIE5A); // Output Compare A Match Interrupt Enable
}
/* System clock ISR */
ISR(TIMER5_COMPA_vect)
{
    seconds += 1;
}