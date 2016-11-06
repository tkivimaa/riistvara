#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_
#define VER_LIBC "avr-libc version: %S\n"
#define VER_FW "Version: %S built on: %S %S\n"

const char monthList[] PROGMEM = "Enter Month name first letter >";
const char kuu1[] PROGMEM = "January";
const char kuu2[] PROGMEM = "February";
const char kuu3[] PROGMEM = "March";
const char kuu4[] PROGMEM = "April";
const char kuu5[] PROGMEM = "May";
const char kuu6[] PROGMEM = "June";
PGM_P const nameMonth[] PROGMEM = {kuu1, kuu2, kuu3, kuu4, kuu5, kuu6};
const char STUD_NAME[] PROGMEM = "Taavi Kivimaa";

#endif /* _HMI_MSG_H_ */