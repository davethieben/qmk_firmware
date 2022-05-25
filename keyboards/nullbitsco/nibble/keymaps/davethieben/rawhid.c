/* Copyright 2022 Dave Thieben (https://github.com/davethieben)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "quantum.h"
#include "rawhid.h"

/*
references:
    keyboards\massdrop\ctrl\keymaps\foxx1337\keymap.c:248
    keyboards\massdrop\ctrl\keymaps\foxx1337\hid_protocol.h
*/

void raw_hid_receive(uint8_t *data, uint8_t length)
{
    // uint8_t command[length] = {0};
    // memcpy(command, data, length);
    char input[32] = {0};
    uint8_t input_len = length > 32 ? 32 : length;
    memcpy(input, data, input_len);

    char message[32];
    snprintf(message, sizeof(message), "%d:%s", length, input);
    set_oled_status(message);

    char response[33] = "Hello from Nibble";
    raw_hid_send((uint8_t *)response, strlen(response));
}

/* WIP:
void raw_hid_receive(uint8_t *data, uint8_t length)
{
    dprintf("raw_hid_receive: %s", data);

    uint8_t *command_id = &(data[0]);
    uint8_t *command_data = &(data[1]);
    switch (*command_id)
    {
        case command_oled_enable:
        {
            if (command_data[0])
                oled_off();
            else
                oled_on();
            break;
        }
        case command_oled_set_message:
        {
            // int msglen = strlen(command_data);
            set_oled_status((char *)command_data);
            break;
        }
        default:
        {
            // used for debugging when building your host application by returning all data back to the host.
            raw_hid_send(data, length);
        }
    }
}
*/
