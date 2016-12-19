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
#ifndef _RFID_H_
#define _RFID_H_

typedef struct card {
    uint8_t uid_size;
    uint8_t uid[10];
    char *user;
    struct card *next;
} card_t;

extern card_t *head;
extern card_t* rfid_find_card(const card_t *card);
extern void rfid_add_card(const card_t *card);
extern void rfid_list_cards(void);
extern void rfid_remove_card(const char *user);

#endif /* _RFID_H_ */