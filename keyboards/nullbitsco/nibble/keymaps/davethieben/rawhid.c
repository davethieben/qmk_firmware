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
#include "rgblight.h"

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

void raw_hid_send_log(uint8_t command_id, char* message)
{
    uint8_t n_buffer[32] = {0};
    memcpy(n_buffer, message, strlen(message));
    raw_hid_send_command(command_id, n_buffer, strlen(message));
}

void raw_hid_send_debug(char* message)
{
    raw_hid_send_log(command_log_debug, message);
}

void raw_hid_receive(uint8_t *data, uint8_t length)
{
    if (length < 1)
        return;

    uint8_t command = data[0];
    data = data + 1;
    length = length - 1;

    uint8_t n_buffer[32] = {0};
    char s_buffer[32] = {0};

    switch (command)
    {
    case command_ping:
        raw_hid_send_debug("Hello from Nibble");
        break;

    // RGB commands:
    case command_rgb_toggle:
        rgblight_toggle();
        raw_hid_send_debug("OK");
        break;

    case command_rgb_set_mode:
        rgblight_mode(data[0]);
        raw_hid_send_debug("OK");
        break;

    // OLED commands:
    case command_oled_toggle:
        oled_toggle();
        raw_hid_send_debug("OK");
        break;

    case command_oled_diagnostic:
        raw_hid_send_log(command_log_info, "OLED Data: ");
        break;

    case command_oled_set_message:
        memcpy(s_buffer, data, length);
        set_oled_status(s_buffer);
        raw_hid_send_debug("OK");
        break;

    default:
        memcpy(n_buffer, "Unknown command", 15);
        raw_hid_send_command(command_log_error, n_buffer, 15);
        break;
    }
}

void raw_hid_send_command(uint8_t command_id, uint8_t *data, uint8_t length)
{
    uint8_t output[32] = {0};
    output[0] = command_id;
    memcpy(output + 1, data, length);
    raw_hid_send(output, 32);
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
