#include <avr/io.h>
#include <stdio.h>
#include "../lib/andygock_avr-uart/uart.h"

/* http://www.cs.mun.ca/~rod/Winter2007/4723/notes/serial/serial.html */

int uart0_putchar(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        uart0_putchar('\r', stream);
    }

    uart0_putc(c);
    return 0;
}

int uart0_getchar(FILE *stream)
{
    (void) stream;
    return (unsigned char)uart0_getc();
}
int uart3_putchar(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        uart3_putchar('\r', stream);
    }

    uart3_putc(c);
    return 0;
}