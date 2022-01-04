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
#include "oled_display.h"
#include "layers.h"

/* Quantum OLED driver functions: */
oled_rotation_t oled_init_user(oled_rotation_t rotation)
{
    oled_timer = timer_read32();
    set_oled_mode(OLED_MODE_IDLE);
    return OLED_ROTATION;
}

bool oled_task_user(void)
{
    if (timer_elapsed(oled_timer) >= OLED_STATUS_TIMEOUT)
    {
        set_oled_mode(OLED_MODE_IDLE);
    }
    render_frame();
    return false;
}
/* :Quantum OLED driver functions */


void set_oled_mode(oled_mode_t mode)
{
    oled_mode = mode;
}

void oled_process_record_encoder(uint16_t keycode)
{
    oled_timer = timer_read32();
    switch (keycode)
    {
    case KC_VOLU:
        set_oled_mode(OLED_MODE_VOLUME_UP);
        break;
    case KC_VOLD:
        set_oled_mode(OLED_MODE_VOLUME_DOWN);
        break;
    default:
        set_oled_mode(OLED_MODE_IDLE);
        break;
    }
}

void oled_process_record_keymap(uint16_t keycode)
{
    oled_timer = timer_read32();
    if (rgblight_is_enabled())
    {
        set_oled_mode(OLED_MODE_RGB_OFF);
    }
    else
    {
        set_oled_mode(OLED_MODE_RGB_ON);
    }
}

void render_layer(void)
{
    switch (get_highest_layer(layer_state))
    {
    case _MAIN:
        oled_write_P(PSTR("Base "), false);
        break;
    case _NAV:
        oled_write_P(PSTR("Nav  "), false);
        break;
    case _AUX:
        oled_write_P(PSTR("Aux  "), false);
        break;
    default:
        oled_write_P(PSTR("Unkn "), false);
        break;
    }
}

void render_wpm(void)
{
    uint8_t n = get_current_wpm();
    char wpm_counter[4];
    wpm_counter[3] = '\0';
    wpm_counter[2] = '0' + n % 10;
    wpm_counter[1] = '0' + (n /= 10) % 10;
    wpm_counter[0] = '0' + n / 10;
    oled_write(wpm_counter, false);
}

void render_idle(void)
{
    oled_write_P(PSTR("| Nibble    "), false);
    render_layer();
    oled_write_P(PSTR("  |\n"), false);

    oled_write_P(PSTR("|                  |\n"), false);

    oled_write_P(PSTR("|    WPM: "), false);
    render_wpm();
    oled_write_P(PSTR("      |\n"), false);

    oled_write_P(PSTR("|                  |\n"), false);

}

void render_status_mode_message(void)
{
    // Printing state icon with message
    oled_write_P(oled_mode_icons[oled_mode][0], false);
    oled_write_P(PSTR("\n"), false);
    oled_write_P(oled_mode_icons[oled_mode][1], false);
    oled_write_P(PSTR("  "), false);
    oled_write_P(oled_mode_messages[oled_mode], false);
    oled_write_P(PSTR("\n"), false);
    oled_write_P(oled_mode_icons[oled_mode][2], false);
    oled_write_P(PSTR("\n\n"), false);
}

void render_frame(void)
{
    switch (oled_mode)
    {
    case OLED_MODE_VOLUME_UP:
    case OLED_MODE_VOLUME_DOWN:
    case OLED_MODE_RGB_ON:
    case OLED_MODE_RGB_OFF:
        render_status_mode_message();
        break;
    default:
        render_idle();
        break;
    }
}
