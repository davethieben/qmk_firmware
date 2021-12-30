/* Copyright 2021 Jay Greco
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
#include QMK_KEYBOARD_H

#ifdef OLED_ENABLE
#include "oled_display.h"
#endif

enum layer_names
{
    _MAIN,
    _NAV,
    _MEDIA
};

enum custom_keycodes
{
    KC_CUST = SAFE_RANGE,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_MAIN] = LAYOUT_all(
                KC_GESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,      KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS, KC_EQL,    KC_BSPC,                KC_HOME,
    KC_MUTE,    KC_TAB,     KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,      KC_L,       KC_U,       KC_Y,       KC_SCLN,    KC_LBRC, KC_RBRC,   KC_BSLS,                KC_END,
    KC_CALC,    MO(_NAV),   KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,      KC_N,       KC_E,       KC_I,       KC_O,       KC_QUOT,            KC_ENT,                 KC_PGUP,
    KC_F14,     KC_LSFT,    KC_F13,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_K,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,            KC_RSFT,    KC_UP,      KC_PGDN,
    MO(_MEDIA), KC_LCTL,    KC_LGUI, KC_LALT,                   KC_SPC,             MO(_NAV),               KC_RALT,                         KC_RCTL,   KC_LEFT,    KC_DOWN,    KC_RGHT
  ),
  [_NAV] = LAYOUT_ansi(
                _______,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,     KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,  KC_F12,     _______,       _______,
    _______,    _______,    _______, _______, _______, _______, _______, _______,   KC_PGUP,    KC_UP,      KC_PGDN,    C(KC_DEL),  KC_DEL,  KC_BSPC,    C(KC_BSPC),    _______,
    _______,    _______,    _______, _______, _______, _______, _______, KC_HOME,   C(KC_LEFT), KC_DOWN,    C(KC_RGHT), KC_END,     _______,             _______,       _______,
    _______,    _______,    _______, _______, _______, _______, _______, KC_LEFT,   _______,    _______,    KC_RGHT,    _______,    _______,             _______,       _______,
    _______,    _______,    _______, _______,                   _______,                        _______,    KC_APP,     _______,    _______,             _______,       _______
  ),
  [_MEDIA] = LAYOUT_ansi(
                RESET,      RGB_TOG, _______, _______, _______, _______, _______,   _______,    _______,    _______,    _______,    _______, _______,    _______,       _______,
    _______,    _______,    _______, _______, _______, _______, _______, _______,   _______,    _______,    _______,    _______,    _______, _______,    _______,       _______,
    _______,    _______,    _______, _______, _______, _______, _______, _______,   _______,    _______,    _______,    _______,    _______,             _______,       _______,
    _______,    _______,    _______, _______, _______, _______, _______, _______,   _______,    _______,    _______,    _______,    _______,             _______,       _______,
    _______,    _______,    _______, _______,                   _______,                        _______,    _______,    _______,    _______,             _______,       _______
  ),
};
// clang-format on

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    oled_timer = timer_read32();
    set_oled_mode(OLED_MODE_IDLE);
    return OLED_ROTATION_0;
}

bool oled_task_user(void) {
    if (timer_elapsed(oled_timer) >= 3000) {
        set_oled_mode(OLED_MODE_IDLE);
    }
    render_frame();
    return false;
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    // Send keystrokes to host keyboard, if connected (see readme)
    // process_record_remote_kb(keycode, record);

    bool ctrl = get_mods() & MOD_MASK_CTRL;

    switch (keycode)
    {
    case RGB_TOG:
        if (record->event.pressed)
        {
#ifdef OLED_ENABLE
            process_record_keymap_oled(keycode);
#endif
        }
        break;

    case KC_GESC:
        if (ctrl && record->event.pressed)
        {
            // TODO - unsigned conversion from 'int' to 'unint8_t' (aka 'unsigned char') .. ?
            // tap_code(LCTL(KC_F4));
            //return false;
        }
        break;

    case KC_CUST: //custom macro
        if (record->event.pressed)
        {
        }
        break;
    }

    return true;
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
}

bool encoder_update_user(uint8_t index, bool clockwise)
{
    //if (get_highest_layer(layer_state|default_layer_state) > 0) { // not default layer

    if (layer_state_is(1))
    {
        if (clockwise)
        {
            tap_code(KC_VOLU);
#ifdef OLED_ENABLE
            process_record_encoder_oled(KC_VOLU);
#endif
        }
        else
        {
            tap_code(KC_VOLD);
#ifdef OLED_ENABLE
            process_record_encoder_oled(KC_VOLD);
#endif
        }
    }

    else if (layer_state_is(2))
    {
        change_RGB(clockwise);
    }

    else // Layer 0 - base layer
    {
        if (clockwise)
        {
            tap_code(KC_WH_D); // mouse wheel down
        }
        else
        {
            tap_code(KC_WH_U); // mouse wheel up
        }
    }

    return false;
}

void matrix_init_user(void)
{
    // Initialize remote keyboard, if connected (see readme)
    matrix_init_remote_kb();
}

void matrix_scan_user(void)
{
    // Scan and parse keystrokes from remote keyboard, if connected (see readme)
    matrix_scan_remote_kb();
}
