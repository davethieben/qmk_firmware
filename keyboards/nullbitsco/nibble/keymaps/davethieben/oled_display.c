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

static const char EMPTY_STATUS[] = "                    ";

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

    if (timer_elapsed(oled_status_timer) >= OLED_STATUS_TIMEOUT)
    {
        reset_oled_status();
    }

    render_frame();
    return false;
}
/* :Quantum OLED driver functions */


void set_oled_mode(oled_mode_t mode)
{
    oled_mode = mode;
    dprintf("OLED Mode: %04X", mode);
}

void set_oled_status(char status[])
{
    oled_status_timer = timer_read32();
    strcpy(oled_status, status);

    dprintf("OLED Status: %s", oled_status);
}

void reset_oled_status(void)
{
    strcpy(oled_status, EMPTY_STATUS);
    dprintf("OLED Status Reset");
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
    switch (keycode)
    {
    case RGB_TOG:
        oled_timer = timer_read32();
        if (rgblight_is_enabled())
        {
            set_oled_mode(OLED_MODE_RGB_OFF);
        }
        else
        {
            set_oled_mode(OLED_MODE_RGB_ON);
        }
        break;

    // case OLED_TOG:
    //     if (is_oled_on())
    //         oled_off();
    //     else
    //         oled_on();
    //     break;
    }
}

void render_layer(void)
{
    switch (get_highest_layer(layer_state))
    {
    case _MAIN:
        oled_write_P(PSTR("Base"), false);
        break;
    case _NAV:
        oled_write_P(PSTR("NAV "), false);
        break;
    case _NUM:
        oled_write_P(PSTR("NUM "), false);
        break;
    case _AUX:
        oled_write_P(PSTR("AUX "), false);
        break;
    default:
        oled_write_P(PSTR("????"), false);
        break;
    }
}

#ifdef WPM_ENABLE
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
#endif

void render_idle(void)
{
    // OLED can contain 4 rows of 20 chars max

    // line 1:
    oled_write_P(PSTR("Nibble        "), false); // 14 chars
    render_layer(); // 4 chars
    oled_write_line("");

    // line 2:
    oled_write_line(oled_status);

    // line 3:
    //oled_write_line("");

    // line 4:
    //char *current_time = get_time();
    //oled_write_line(current_time);

    //oled_write_P(PSTR("    WPM: "), false);
    //render_wpm();
    //oled_write_line("");

}

/** needs fixed - where to get current date/time?
char* get_time()
{
    time_t rawtime;
    time (&rawtime);

    struct tm * timeinfo;
    timeinfo = localtime(&rawtime);

    char buffer[20];
    strftime(buffer, 20,"%a %h %d %I:%M %p",timeinfo);
    return buffer;
}*/

void oled_write_line(char line[])
{
    oled_write(line, false);

    int remaining = 19 - strlen(line);
    while (remaining > 0)
    {
        oled_write_char(' ', false);
        remaining--;
    }
    oled_write_P(PSTR("\n"), false);
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
