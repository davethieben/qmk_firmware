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
#include "encoder.h"
#include "oled_display.h"
#include "layers.h"

bool encoder_update_user(uint8_t index, bool clockwise)
{
    //if (get_highest_layer(layer_state|default_layer_state) > 0) { // not default layer

    if (layer_state_is(_NAV))
    {
        if (clockwise)
        {
            tap_code(KC_VOLU);
            oled_process_record_encoder(KC_VOLU);
        }
        else
        {
            tap_code(KC_VOLD);
            oled_process_record_encoder(KC_VOLD);
        }
    }

    else if (layer_state_is(_AUX))
    {
        change_RGB(clockwise);
    }

    else // base layer
    {
        if (clockwise)
        {
            tap_code(KC_MS_WH_UP);
        }
        else
        {
            tap_code(KC_MS_WH_DOWN);
        }
        set_oled_status("Scrolling...");
    }

    return false;
}

// RGB config, for changing RGB settings on non-VIA firmwares
void change_RGB(bool clockwise)
{
    bool shift = get_mods() & MOD_MASK_SHIFT;
    bool alt = get_mods() & MOD_MASK_ALT;
    bool ctrl = get_mods() & MOD_MASK_CTRL;

    if (clockwise)
    {
        if (alt)
        {
            rgblight_increase_hue();
        }
        else if (ctrl)
        {
            rgblight_increase_val();
        }
        else if (shift)
        {
            rgblight_increase_sat();
        }
        else
        {
            rgblight_step();
        }
    }
    else
    {
        if (alt)
        {
            rgblight_decrease_hue();
        }
        else if (ctrl)
        {
            rgblight_decrease_val();
        }
        else if (shift)
        {
            rgblight_decrease_sat();
        }
        else
        {
            rgblight_step_reverse();
        }
    }

    static char rgbStatusLine1[26] = {0};
    snprintf(rgbStatusLine1, sizeof(rgbStatusLine1), "RGB Mode: %d", rgblight_get_mode());
    set_oled_status(rgbStatusLine1);

    // static char rgbStatusLine2[26] = {0};
    // snprintf(rgbStatusLine2, sizeof(rgbStatusLine2), "h:%d s:%d v:%d", rgblight_get_hue(), rgblight_get_sat(), rgblight_get_val());
    // set_oled_status(rgbStatusLine2);
}
