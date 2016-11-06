#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "uart.h"
#include "print_helper.h"
#include "hmi_msg.h"
#include "../lib/hd44780_111/hd44780.h"

#define BLINK_DELAY_MS 100
int main (void)
{
    /* set pin 3 of PORTA for output*/
    DDRA |= _BV(DDA3);
    /* Init error console as stderr in UART3 and print user code info */
    uart3_init();
    uart0_init();
    stdin = stdout = &uart0_io;
    stderr = &uart3_out;
    lcd_init();
    lcd_clrscr();
    lcd_puts_P(STUD_NAME);
    fprintf_P(stdout, STUD_NAME, "\n");
    fprintf_P(stderr, PSTR(VER_FW), PSTR(GIT_DESCR), PSTR(__DATE__),
              PSTR(__TIME__));
    fprintf_P(stderr, PSTR(VER_LIBC), PSTR(__AVR_LIBC_VERSION_STRING__));
    print_ascii_tbl(stdout);
    unsigned char ascii[128] = {0};

    for (unsigned char i = 0; i < 128; i++) {
        ascii[i] = i;
    }

    print_for_human(stdout, ascii, 128);

    while (1) {
        /* set pin 3 high to turn led on */
        char month;
        fprintf_P(stdout, monthList);
        fscanf(stdin, "%c", &month);
        fprintf(stdout, "%c\n", month);
        lcd_clr(LCD_ROW_2_START, 16);
        lcd_goto(LCD_ROW_2_START);

        for (int i = 0; i < 6; i++) {
            if (!strncmp_P(&month, (PGM_P)pgm_read_word(&nameMonth[i]), 1)) {
                lcd_putc(' ');
                fprintf_P(stdout, (PGM_P)pgm_read_word(&nameMonth[i]));
                lcd_puts_P((PGM_P)pgm_read_word(&nameMonth[i]));
                lcd_putc(' ');
                fprintf(stdout, "\n");
            }
        }

        int loendur = 0;

        while (loendur < 16) {
            lcd_goto(LCD_ROW_2_START);
            lcd_putc(' ');
            loendur++;
        }

        PORTA |= _BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
        /* set pin 3 low to turn led off */
        PORTA &= ~_BV(PORTA3);
        _delay_ms(BLINK_DELAY_MS);
    }
}