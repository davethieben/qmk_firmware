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

void raw_hid_write(char *output, size_t length)
{
    // TODO - if length > 32?
    if (length > 32)
        length = 32;

    uint8_t response[32] = {0};
    memcpy(response, output, length);
    raw_hid_send(response, 32);
}

void raw_hid_echo(uint8_t *data, uint8_t length)
{
    char input[32] = {0};
    uint8_t input_len = length > 32 ? 32 : length;
    memcpy(input, data, input_len);

    char message[32];
    snprintf(message, sizeof(message), "%d:%s", length, input);
    set_oled_status(message);

    raw_hid_write("Hello from Nibble", 32);
}

void set_oled_status_cpy(uint8_t *data, uint8_t length)
{
    char message[32];
    memcpy(message, data, length);
    set_oled_status(message);
}

void raw_hid_receive(uint8_t *data, uint8_t length)
{
    switch (*data)
    {
    case command_ping:
        raw_hid_write("Hello from Nibble", 32);
        break;

    case command_oled_toggle:
        break;

    case command_oled_set_message:
        set_oled_status_cpy(data + 1, length - 1);
        break;

    case command_rgb_toggle:
        break;

    case command_rgb_set_mode:
        break;

    default:
        raw_hid_write("Unknown command", 32);
        break;
    }
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
