#include <avr/pgmspace.h>
#include "hmi_msg.h"

const char monthList[] PROGMEM = "Enter Month name first letter >";
const char studName[] PROGMEM = "Taavi Kivimaa";
const char uptime[] PROGMEM = "Uptime: %lu s";
static const char month1[] PROGMEM = "January";
static const char month2[] PROGMEM = "February";
static const char month3[] PROGMEM = "March";
static const char month4[] PROGMEM = "April";
static const char month5[] PROGMEM = "May";
static const char month6[] PROGMEM = "June";
PGM_P const nameMonth[] PROGMEM = {month1, month2, month3, month4, month5, month6};


const char help_cmd[] PROGMEM = HELP_CMD;
const char help_help[] PROGMEM = HELP_HELP;
const char ver_cmd[] PROGMEM = VER_CMD;
const char ver_help[] PROGMEM = VER_HELP;
const char ascii_cmd[] PROGMEM = ASCII_CMD;
const char ascii_help[] PROGMEM = ASCII_HELP;
const char month_cmd[] PROGMEM = MONTH_CMD;
const char month_help[] PROGMEM = MONTH_HELP;
const char avail_cmd[] PROGMEM = AVAIL_CMD;
const char invalid_cmd[] PROGMEM = INVALID_CMD;
const char invalid_args[] PROGMEM = INVALID_ARGS;
