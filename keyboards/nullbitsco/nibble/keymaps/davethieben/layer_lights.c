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
#include "layers.h"
#include "layer_lights.h"

void layer_lights_init(void)
{
    // Enable the LED layers
    rgblight_layers = layer_lights;
}

void activate_default_layer(layer_state_t state)
{
    //rgblight_set_layer_state(1, layer_state_cmp(state, _MAIN));
    rgblight_set_layer_state(0, false); // turn off for base layer
}

void activate_layer(layer_state_t state)
{
    rgblight_set_layer_state(0, layer_state_cmp(state, _NAV));
    rgblight_set_layer_state(1, layer_state_cmp(state, _AUX));
}
