#include <avr/io.h>
#include <stdio.h>
#include "../lib/andygock_avr-uart/uart.h"

#define UART_STATUS_MASK    0x00FF

/* http://www.cs.mun.ca/~rod/Winter2007/4723/notes/serial/serial.html */

int uart0_putchar_wrapped(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        uart0_putchar_wrapped('\r', stream);
    }

    uart0_putc(c);
    return 0;
}

int uart0_getchar_wrapped(FILE *stream)
{
    (void) stream;
    return uart0_getc() & UART_STATUS_MASK;
}
int uart3_putchar_wrapped(char c, FILE *stream)
{
    (void) stream;

    if (c == '\n') {
        uart3_putchar_wrapped('\r', stream);
    }

    uart3_putc(c);
    return 0;
}