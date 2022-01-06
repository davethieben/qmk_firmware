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
#include QMK_KEYBOARD_H
#include "print.h"

#include "layers.h"
#include "layer_lights.h"
#include "oled_display.h"

// clang-format off
//      keycode functions:          https://docs.qmk.fm/#/feature_layers
//          MO(layer) - momentarily activates the layer
//          LM(layer, mod) - momentarily activates the layer with 'mod' active (MOD_LCTL, MOD_LSFT, MOD_LALT, MOD_LGUI)
//          LT(layer, kc) - momentarily activates layer when held, and sends kc when tapped
//          TG(layer) - toggle layer
//          TO(layer) - toggle layer and turns off all others
//          TT(layer) - layer tap-toggle (configure TAPPING_TOGGLE in config.h for tap count)
enum custom_keycodes
{
    KC_CUST     = SAFE_RANGE,
    // custom codes with handlers in process_record_user:
    SUPER_ALT_TAB,
    // alias codes:
    ROTARY      = KC_MUTE,
    ACTION_1    = KC_CALC,
    ACTION_2    = MO(_AUX),
    ACTION_3    = SUPER_ALT_TAB,
    CAPS_LOCK   = MO(_NAV),
    LEFT_SPACE  = MT(MOD_LCTL, KC_ENT),
    RGHT_SPACE  = KC_SPC,
    FUNC_SPACE  = TT(_NAV),
};

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_MAIN] = LAYOUT_all(
                KC_GESC,    KC_1,       KC_2,    KC_3,    KC_4,    KC_5,    KC_6,      KC_7,       KC_8,        KC_9,       KC_0,       KC_MINS, KC_EQL,    KC_BSPC,                KC_HOME,
    ROTARY,     KC_TAB,     KC_Q,       KC_W,    KC_F,    KC_P,    KC_G,    KC_J,      KC_L,       KC_U,        KC_Y,       KC_SCLN,    KC_LBRC, KC_RBRC,   KC_BSLS,                KC_END,
    ACTION_1,   CAPS_LOCK,  KC_A,       KC_R,    KC_S,    KC_T,    KC_D,    KC_H,      KC_N,       KC_E,        KC_I,       KC_O,       KC_QUOT,            KC_ENT,                 KC_PGUP,
    ACTION_2,   KC_LSFT,    LEFT_SPACE, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_K,       KC_M,        KC_COMM,    KC_DOT,     KC_SLSH,            KC_RSFT,    KC_UP,      KC_PGDN,
    ACTION_3,   KC_LCTL,    KC_LGUI,    KC_LALT,                   RGHT_SPACE,         FUNC_SPACE,              KC_RALT,                         KC_RCTL,   KC_LEFT,    KC_DOWN,    KC_RGHT
  ),
  [_NAV] = LAYOUT_all(
                _______,    KC_F1,      KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,     KC_F7,      KC_F8,       KC_F9,      KC_F10,     KC_F11,  KC_F12,     _______,                _______,
    _______,    _______,    C(KC_Q),    _______, _______, _______, _______, _______,   KC_PGUP,    KC_UP,       KC_PGDN,    C(KC_DEL),  KC_DEL,  KC_BSPC,    C(KC_BSPC),             _______,
    _______,    _______,    _______,    _______, _______, _______, _______, KC_HOME,   C(KC_LEFT), KC_DOWN,     C(KC_RGHT), KC_END,     _______,             _______,                _______,
    _______,    _______,    _______,    C(KC_Z), C(KC_X), C(KC_C), C(KC_V), _______,   KC_LEFT,    SUPER_ALT_TAB, _______,  KC_RGHT,    _______,             _______,    _______,    _______,
    _______,    _______,    _______,    _______,                   _______,            _______,                 KC_APP,                          _______,    A(KC_LEFT), _______,    A(KC_RGHT)
  ),
  [_AUX] = LAYOUT_all(
                RESET,      _______,    _______, _______, _______, _______, _______,   _______,    _______,     _______,    _______,    _______, _______,    _______,                _______,
    RGB_TOG,    DEBUG,      _______,    _______, _______, _______, _______, _______,   _______,    _______,     _______,    _______,    _______, _______,    _______,                _______,
    _______,    _______,    _______,    _______, _______, _______, _______, _______,   _______,    _______,     _______,    _______,    _______,             _______,                _______,
    _______,    _______,    _______,    _______, _______, _______, _______, _______,   _______,    _______,     _______,    _______,    _______,             _______,    _______,    _______,
    _______,    _______,    _______,    _______,                   _______,            _______,                 _______,                         _______,    _______,    _______,    _______
  ),
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    dprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n",
            keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif

    const uint8_t mods = get_mods();
    //bool shift = mods & MOD_MASK_SHIFT;
    //bool gui = mods & MOD_MASK_GUI;
    bool ctrl = mods & MOD_MASK_CTRL;

    switch (keycode)
    {
    case RGB_TOG:
        if (record->event.pressed)
        {
            oled_process_record_keymap(keycode);
        }
        break;

    case SUPER_ALT_TAB:
        if (record->event.pressed)
        {
            if (!is_alt_tab_active)
            {
                is_alt_tab_active = true;
                register_code(KC_LALT);
            }
            alt_tab_timer = timer_read();
            register_code(KC_TAB);
        }
        else
        {
            unregister_code(KC_TAB);
        }
        break;

    case KC_BSLS:
        if (record->event.pressed && ctrl)
        {
            tap_code(KC_TAB);
            return false;
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

layer_state_t default_layer_state_set_user(layer_state_t state)
{
    activate_default_layer(state);

    return state;
}

layer_state_t layer_state_set_user(layer_state_t state)
{
    activate_layer(state);

    return state;
}

void matrix_init_user(void)
{
    // keyboard init:
    layer_lights_init();

#ifdef REMOTEKB_ENABLE
    // Initialize remote keyboard, if connected (see readme)
    matrix_init_remote_kb();
#endif
}

void matrix_scan_user(void)
{
    if (is_alt_tab_active && timer_elapsed(alt_tab_timer) > 500)
    {
        unregister_code(KC_LALT);
        is_alt_tab_active = false;
    }

#ifdef REMOTEKB_ENABLE
    // Scan and parse keystrokes from remote keyboard, if connected (see readme)
    matrix_scan_remote_kb();
#endif
}
