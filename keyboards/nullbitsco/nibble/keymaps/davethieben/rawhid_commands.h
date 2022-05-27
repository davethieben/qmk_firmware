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
#if NET
namespace MKHighway {
public
#else
#pragma once
#endif

enum hid_command_id
{
    command_ping = 0x40,
    command_log_debug,
    command_log_info,
    command_log_error,

    command_key_send = 0x60,

    command_rgb_toggle = 0x70,
    command_rgb_set_mode,

    command_oled_toggle = 0x80,
    command_oled_diagnostic,
    command_oled_set_message,

};

#if NET
}
#endif
