#include "smart_layer.h"
#include "keycodes.h"

/* -------- Default Smart Layers --------
 * These functions are for generic behaviorgeneric behavior * This is for mod-order-agnostic oneshot layers!
 * These will ignore nshot mods and disable the layer after one tap for everything else.
 * There's just the one method here!
*/

bool smart_layer_enable(uint8_t layer) {
    layer_on(layer);
    return true;
}

bool smart_layer_disable(uint8_t layer){
    layer_off(layer);
    return false;
}

bool smart_oneshot_process(uint16_t keycode, keyrecord_t *record, uint8_t layer){
    switch (keycode) {
        case OS_LSFT ... OSR_SFT:
            break;
        default:
            if (!record->event.pressed) {
                return smart_layer_disable(layer);
            }
            break;
    }
    return true;
}

/* -------- Custom Smart Layers --------
 * Each of these layers has fancy logic for when the layer should be disabled.
 * Create a custom process function, a custom enable function, or both!
*/


/* -------- Number Mode -------- */
bool num_mode_process(uint16_t keycode, keyrecord_t *record) {
    // Assess if we should exit layermode or continue processing normally.
    switch (keycode) {
        case OS_LSFT ... OSR_SFT:
        case KC_1 ... KC_0:
        case LGUI_T(KC_8):
        case LALT_T(KC_4):
        case LCTL_T(KC_5):
        case LSFT_T(KC_6):
        case KC_DLR:
        case KC_QUOT:
        case KC_SLSH:
        case KC_SCLN:
        case KC_COLN:
        case KC_COMM:
        case KC_DOT:
        case KC_MINS:
        case KC_ASTR:
        case KC_EQL:
        case KC_PLUS:
        case BRCKETS:
        case KC_N:
        case KC_V:
        case KC_X:
            // process the code and stay in the mode *dabs*
            break;
        default:
            // All other keys disable the layer mode on keyup.
            if (!record->event.pressed) {
                return smart_layer_disable(_NUM);
            }
            break;
    }
    return true;
}

/* -------- Function Mode -------- */

bool func_mode_process(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_F3:
        case F5_TH:
        case KC_F8:
        case KC_F10:
        case F11_TH:
        case OS_LSFT ... OSR_SFT:
            break;
        default:
            if (!record->event.pressed) {
                return smart_layer_disable(_FUNC);
            }
            break;
    }
    return true;
}

/*--------- Nav Mode ---------------*/
// Turn nav mode on. To be called from a custom keycode.
static uint16_t nav_mode_timer;
bool nav_mode_enable(keyrecord_t *record) {
    // On keydown, turn on nav and start the timer.
    if (record->event.pressed) {
        layer_on(_NAV);
        nav_mode_timer = timer_read();
        // not enabling smart layer yet--depends how long the key's down!
        return false;
    } 

    // on keyup, if held for less than tapping term, activate!
    // The layer is already on at this point, so no further action needed.
    if (timer_elapsed(nav_mode_timer) < TAPPING_TERM) {
        // Tapping enables layer mode
        return true;
    } 

    // otherwise, treat it like a normal held layer and turn off the layer on keyup.
    layer_off(_NAV);
    return false;
}

bool nav_mode_process(uint16_t keycode, keyrecord_t *record){
    // Assess if we should exit layermode or continue processing normally.
    switch (keycode) {
        case OS_LSFT ... OSR_SFT:
        case ML_LCTL ... ML_LGUI:
        case KC_HOME ... KC_UP:
        case KC_MS_U ... KC_ACL2:
        case CLEAR:
            break;
        default:
            if (!record->event.pressed) {
                return smart_layer_disable(_NAV);
            }
            break;
    }
    return true;
}

