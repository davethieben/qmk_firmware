/* Copyright 2021 Jose Luis Adelantado Torres
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
#pragma once

typedef enum {
    OLED_MODE_IDLE = 0,
    OLED_MODE_VOLUME_UP = 1,
    OLED_MODE_VOLUME_DOWN = 2,
    OLED_MODE_RGB_ON = 3,
    OLED_MODE_RGB_OFF = 4,
} oled_mode_t;

static const char PROGMEM oled_mode_messages[5][15] = {
    "",
    "Volume Up",
    "Volume Down",
    "RGB ON",
    "RGB OFF"};

static const char PROGMEM oled_mode_icons[5][3][5] = {
    {{128, 129, 130, 131, 0}, {160, 161, 162, 163, 0}, {192, 193, 194, 195, 0}},
    {{132, 133, 134, 135, 0}, {164, 165, 166, 167, 0}, {196, 197, 198, 199, 0}},
    {{136, 137, 138, 139, 0}, {168, 169, 170, 171, 0}, {200, 201, 202, 203, 0}},
    {{140, 141, 142, 143, 0}, {172, 173, 174, 175, 0}, {204, 205, 206, 207, 0}},
    {{144, 145, 146, 147, 0}, {176, 177, 178, 179, 0}, {208, 209, 210, 211, 0}}};

uint16_t oled_timer;
oled_mode_t oled_mode;

void set_oled_mode(oled_mode_t mode);

void oled_process_record_encoder(uint16_t keycode);

void oled_process_record_keymap(uint16_t keycode);

void render_wpm(void);

void render_idle(void);

void render_status_mode_message(void);

void render_frame(void);
