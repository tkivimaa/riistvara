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
#define READ_CMD "read"
#define READ_HELP "Read and display card info"
#define ADD_CMD "add"
#define ADD_HELP "Add a new card to the system. Usage: add <name>"
#define REMOVE_CMD "remove"
#define REMOVE_HELP "Remove a card from the system. Usage: remove <name>"
#define LIST_CMD "list"
#define LIST_HELP "List all added cards"
#define ACCESS_DENIED_MSG "Access denied!"
#define CARD_EXISTS_MSG "The following card is already added: "
#define OUT_OF_MEMORY_MSG "No memory. Remove cards"
#define NO_CARDS_ADDED_MSG "Card not added"
#define CARD_NOT_FOUND_MSG "Card not found"
#define LINKED_LIST_ERROR_MSG "Card already removed"
#define UNABLE_TO_DETECT_CARD_MSG "Unable to detect card."
#define CARD_SELECTED_MSG "Card selected!"
#define UID_SIZE_MSG "UID size: 0x%02X"
#define UID_SAK_MSG "UID sak: 0x%02X"
#define CARD_UID_MSG "Card UID: "
#define CARD_NOT_SELECTED "Unable to select card.\n"

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
extern const char read_cmd[];
extern const char read_help[];
extern const char add_cmd[];
extern const char add_help[];
extern const char remove_cmd[];
extern const char remove_help[];
extern const char list_cmd[];
extern const char list_help[];
extern const char access_denied_msg[];

#endif /* _HMI_MSG_H_ */