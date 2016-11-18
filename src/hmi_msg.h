#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_
#define VER_LIBC "avr-libc version: %S\n"
#define VER_FW "Version: %S built on: %S %S\n"

const char monthList[] PROGMEM = "Enter Month name first letter >";
const char month1[] PROGMEM = "January";
const char month2[] PROGMEM = "February";
const char month3[] PROGMEM = "March";
const char month4[] PROGMEM = "April";
const char month5[] PROGMEM = "May";
const char month6[] PROGMEM = "June";
PGM_P const nameMonth[] PROGMEM = {month1, month2, month3, month4, month5, month6};
const char studName[] PROGMEM = "Taavi Kivimaa";
const char uptime[] PROGMEM = "Uptime: %lu s";

#endif /* _HMI_MSG_H_ */