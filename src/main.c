/*This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. */
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <time.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "print_helper.h"
#include "hmi_msg.h"
#include "uart-wrapper.h"
#include "cli_microrl.h"
#include "rfid.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/helius_microrl/microrl.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/matejx_avr_lib/mfrc522.h"

#define BAUD 9600
#define UART_STATUS_MASK 0x00FF
#define LED_START DDRA |= _BV(DDA3)
#define LED_SWITCH PORTA ^= _BV(PORTA3)
#define DOOR_START DDRA |= _BV(DDA1)
#define DOOR_OPEN PORTA |= _BV(PORTA1)
#define DOOR_CLOSE PORTA &= ~_BV(PORTA1)


//microrl with object and pointer
static microrl_t rl;
static microrl_t *prl = &rl;

static inline void displayName(void);
static inline void initialize(void);
static inline void initializeCli(void);
static inline void initSystemClock(void);
static inline void heartbeat(void);
static inline void initializeRfidReader(void);
static inline void doorController(void);
int main (void)
{
    initialize(); //initialize workspace
    displayName(); //Display my name on LED and console
    print_ver(stderr); //Display versions in stderr
    initializeCli(); //Initialize CLI for use
    initializeRfidReader(); //Initialize RFID reader

    while (1) {
        heartbeat();
        // CLI commands are handled in cli_execute()
        microrl_insert_char(prl, cli_get_char());
        doorController();
    }
}
static inline void initializeRfidReader(void)
{
    /* Init RFID-RC522 */
    MFRC522_init();
    PCD_Init();
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
    DOOR_START;
    initSystemClock();
    /* Init error console as stderr in UART3 and print user code info */
    uart3_init(UART_BAUD_SELECT(BAUD, F_CPU)); //Init uart3
    uart0_init(UART_BAUD_SELECT(BAUD, F_CPU)); //init uart0
    sei(); //for interrupts
    stdout = stdin = &uart0_io;
    stderr = &uart3_out;
    lcd_init();
    lcd_clrscr();
}

static inline void displayName(void)
{
    lcd_goto(LCD_ROW_1_START);
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
static inline void doorController()
{
    Uid uid;
    card_t card;
    uint32_t cur_time = time(NULL);
    static uint32_t message_start;
    static uint32_t door_open_start;

    if (PICC_IsNewCardPresent()) {
        PICC_ReadCardSerial(&uid);
        card.uid_size = uid.size;
        memcpy(&card.uid, &uid.uidByte, uid.size);
        card.user = NULL;
        card_t *found_card = rfid_find_card(&card);

        if (found_card) {
            lcd_clr(LCD_ROW_2_START, 16);
            lcd_goto(LCD_ROW_2_START);
            lcd_puts(found_card->user);
            DOOR_OPEN;
        } else {
            DOOR_CLOSE;
            lcd_clr(LCD_ROW_2_START, 16);
            lcd_goto(LCD_ROW_2_START);
            lcd_puts_P(access_denied_msg);
        }

        door_open_start = cur_time;
        message_start = cur_time;
    }

    //Clean screen after 10 seconds
    if ((message_start + 5) < cur_time) {
        message_start = cur_time + 5;
        lcd_goto(LCD_ROW_2_START);
        lcd_clr(LCD_ROW_2_START, 16);
    }

    //Close door after holding open for some time
    if ((door_open_start + 2) < cur_time) {
        DOOR_CLOSE;
    }
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