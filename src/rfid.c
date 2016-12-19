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
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "rfid.h"
#include "hmi_msg.h"


card_t *head = NULL;


void rfid_print_card(const card_t *card)
{
    for (uint8_t i = 0; i < card->uid_size; i++) {
        printf("%02X", card->uid[i]);
    }

    printf(": %s", card->user);
}


card_t* rfid_find_card(const card_t *card)
{
    if (head != NULL) {
        card_t *current;
        current = head;

        while (current != NULL) {
            if ((current->uid_size != card->uid_size) ||
                    !memcmp(current->uid, card->uid, current->uid_size) ||
                    ((card->user != NULL) && !strcmp(current->user, card->user))) {
                return current;
            }

            current = current->next;
        }
    }

    return NULL;
}


void rfid_add_card(const card_t *card)
{
    // Don't add duplicate cards
    card_t *found_card = rfid_find_card(card);

    if (found_card) {
        printf_P(PSTR(CARD_EXISTS_MSG));
        rfid_print_card(found_card);
        return;
    }

    // Card eligible, add it
    card_t *new_card;
    char *new_card_user;
    new_card = malloc(sizeof(card_t));
    new_card_user = malloc(strlen(card->user) + 1);

    if (!new_card || !new_card_user) {
        printf_P(PSTR(OUT_OF_MEMORY_MSG "\n"));
        return;
    }

    // Update data
    new_card->uid_size = card->uid_size;
    memcpy(new_card->uid, card->uid, card->uid_size);
    strcpy(new_card_user, card->user);
    new_card->user = new_card_user;
    new_card->next = NULL;

    // Update list
    if (head == NULL) {
        head = new_card;
    } else {
        card_t *current;
        current = head;

        while (current->next != NULL) {
            current = current->next;
        }

        current->next = new_card;
    }

    return;
}


void rfid_remove_card(const char *user)
{
    card_t *curr;
    card_t *prev;
    curr = head;
    prev = NULL;

    if (head == NULL) {
        printf_P(PSTR(NO_CARDS_ADDED_MSG "\n"));
        return;
    } else {
        while (curr->next != NULL) {
            if (strcmp(curr->user, user) == 0) {
                break;
            }

            prev = curr;
            curr = curr->next;
        }
    }

    if (strcmp(curr->user, user) == 0) {
        if (prev == NULL && curr->next == NULL) {
            // Only one card in list
            free(curr->user);
            free(curr);
            head = NULL;
        } else if (prev == NULL && curr->next != NULL) {
            // First card in list
            head = curr->next;
            free(curr->user);
            free(curr);
        } else if (prev != NULL && curr->next != NULL) {
            // In the middle of the list
            prev->next = curr->next;
            free(curr->user);
            free(curr);
        } else if (prev != NULL && curr->next == NULL) {
            // Last card in list
            prev->next = NULL;
            free(curr->user);
            free(curr);
        } else {
            printf_P(PSTR(LINKED_LIST_ERROR_MSG "\n"));
        }
    } else {
        printf_P(PSTR(CARD_NOT_FOUND_MSG));
    }
}


void rfid_list_cards(void)
{
    if (head == NULL) {
        printf_P(PSTR(NO_CARDS_ADDED_MSG "\n"));
    } else {
        card_t *current;
        current = head;

        while (current->next != NULL) {
            rfid_print_card(current);
            putc('\n', stdout);
            current = current->next;
        }

        rfid_print_card(current);
        putc('\n', stdout);
    }
}
