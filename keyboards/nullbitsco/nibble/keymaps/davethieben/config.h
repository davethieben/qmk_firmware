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
#pragma once

// Referenced custom font
#define OLED_FONT_H "keyboards/nullbitsco/nibble/keymaps/oled_status/glcdfont.c"

// enable setting underglow based on active layer
#define RGBLIGHT_LAYERS

// number of times you have to tap a layer key to toggle it when using TT()
#define TAPPING_TOGGLE 2

// Mousekeys: https://docs.qmk.fm/#/feature_mouse_keys
#define MOUSEKEY_WHEEL_DELAY            300         // Delay between pressing a wheel key and wheel movement
#define MOUSEKEY_WHEEL_INTERVAL         100         // Time between wheel movements
#define MOUSEKEY_WHEEL_MAX_SPEED        16          // Maximum number of scroll steps per scroll action
#define MOUSEKEY_WHEEL_TIME_TO_MAX      40          // Time until maximum scroll speed is reached

#define OLED_ROTATION           OLED_ROTATION_0
#define OLED_STATUS_TIMEOUT     3000
