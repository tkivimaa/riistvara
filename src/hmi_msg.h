#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_
#define VER_LIBC "avr-libc version: %S avr-gcc version: %S\n"
#define VER_FW "Version: %S built on: %S %S\n"

#define HELP_CMD "help"
#define HELP_HELP "Get help"
#define VER_CMD "version"
#define VER_HELP "Print FW version"
#define ASCII_CMD "ascii"
#define ASCII_HELP "print ASCII tables"
#define MONTH_CMD "month"
#define MONTH_HELP "Find matching month from lookup list. Usage: month <string>"
#define AVAIL_CMD "Implemented commands:"
#define INVALID_CMD "Command not implemented. \n Use <help> to get help."
#define INVALID_ARGS "Too few or too many arguments for this command\nUse <help>"

//Non-Command messages
extern PGM_P const nameMonth[];
extern const char monthList[];
extern const char studName[];
extern const char uptime[];
//Command messages
extern const char help_cmd[];
extern const char help_help[];
extern const char ver_cmd[];
extern const char ver_help[];
extern const char ascii_cmd[];
extern const char ascii_help[];
extern const char month_cmd[];
extern const char month_help[];
extern const char avail_cmd[];
extern const char invalid_cmd[];
extern const char invalid_args[];

#endif /* _HMI_MSG_H_ */