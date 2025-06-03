#include "possumvibes.h"
#include <print.h>
#include "keycodes.h"

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  //debug_mouse=true;
}
/**** Processin' Records ****/

static uint16_t next_keycode;
static keyrecord_t next_record;
static uint16_t prev_keycode;
bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    // static uint16_t prev_keycode;
    if (record->event.pressed) {
        // store previous keycode for instant tap decisions
        prev_keycode = next_keycode;

        // Cache the next input for mod-tap decisions
        next_keycode = keycode;
        next_record  = *record;
    }
    return true;
}

// OS-specific logic config
static bool is_linux = true;
// static uint8_t df_layer = 0;

// Smart Layer config
bool _num_mode_active = false;
bool _func_mode_active = false;
bool _nav_mode_active = false;
bool _sym_mode_active = false;
bool _macro_mode_active = false;
bool _alpha_mode_active = false;

bool dead_key_active = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
// #ifdef CONSOLE_ENABLE
    if(record->event.pressed)
        uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
// #endif
    // bool is_shifted =
    //     (get_mods() & MOD_MASK_SHIFT) ||
    //     (get_oneshot_mods() & MOD_MASK_SHIFT);

    // Process the functions
    process_mod_lock(keycode, record);
    process_nshot_state(keycode, record, !is_linux);

    // Process smart layers
    if (_num_mode_active) {
        _num_mode_active = num_mode_process(keycode, record);
    } else if (_func_mode_active) {
        _func_mode_active = func_mode_process(keycode, record);
    } else if (_nav_mode_active){
        _nav_mode_active = nav_mode_process(keycode, record);
    } else if (_sym_mode_active){
        _sym_mode_active = smart_oneshot_process(keycode, record, _SYM);
    } else if (_macro_mode_active){
        _macro_mode_active = smart_oneshot_process(keycode, record, _MACRO);
    } else if (_alpha_mode_active){
        _alpha_mode_active = smart_oneshot_process(keycode, record, _ALPHA2);
    }

    switch (keycode) {
        // Layer Modes
        case NUMMODE: 
            _num_mode_active = smart_layer_enable(_NUM);
            return false;
        case FUNMODE: 
            _func_mode_active = smart_layer_enable(_FUNC);
            return false;
        case NAVMODE: 
            _nav_mode_active = nav_mode_enable(record);
            return false;
        case SYMMODE: 
            _sym_mode_active = smart_layer_enable(_SYM);
            return false;
        case MCRMODE: 
            _macro_mode_active = smart_layer_enable(_MACRO);
            return false;
        case ALPHAMODE: 
            _macro_mode_active = smart_layer_enable(_ALPHA2);
            return false;

        // macros
        case S_CUT:    return C(KC_X);
        case S_COPY:   return C(KC_C);
        case S_PASTE:  return C(KC_V);
        case CLEAR: 
            clear_oneshot_mods();
            clear_mods();
            return false;
        
        case PANIC: {
            // Clear all lock states
            // mods
            clear_oneshot_mods();
            clear_mods();

            // layers
            if (get_oneshot_layer() != 0) {
                clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
            }
            layer_move(_APT);

            // and caps lock/word
            // clear_caps();
            return false;
        }

        case MAGIK:
            // if (record->tap.count && record->event.pressed) {
            //         dead_key_active = true;
            //         return false;
            // }
            if (record->event.pressed) {
                    dead_key_active = true;
                    return false;
            }
            return true;


        default:
            // Handle dead key modifications for other keys
            if (dead_key_active && record->event.pressed) {
                // Dead key is active and another key was pressed
                dead_key_active = false; // Immediately deactivate to prevent multiple modifications
                
                uint16_t base_keycode = keycode;
                if (IS_QK_MOD_TAP(keycode)) {
                    base_keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
                } else if (IS_QK_LAYER_TAP(keycode)) {
                    base_keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
                }
                // Apply your dead key transformation here
                // Example: Convert vowels to accented versions
                switch (base_keycode) {                // Apply your dead key transformation here
                    case KC_S:
                        tap_code(KC_V); // ó
                        return false;
                    case KC_T:
                        tap_code(KC_K); // í
                        return false;
                    case KC_H:
                        tap_code(KC_P);
                        return false; // Prevent the original keycode from being processed
                    case KC_N:
                        tap_code(KC_B); // é
                        return false;
                    case KC_C:
                        tap_code(KC_Z); // ú
                        return false;
                    case KC_Y:
                        tap_code(KC_Q); // ú
                        return false;
                    case KC_W:
                        tap_code(KC_J); // ñ
                        return false;
                    case KC_R:
                        tap_code(KC_X); // ñ
                        return false;
                    case KC_E:
                        tap_code(KC_SCLN); // ñ
                        return false;
                    case KC_A:
                        tap_code(KC_QUOT); // ñ
                        return false;
                    case KC_I:
                        tap_code(KC_COMM); // ñ
                        return false;
                    case KC_U:
                        tap_code16(KC_COLN);
                        return false;
                    // Add more transformations as needed
                    default:
                        // For non-transformable keys, send the original key normally
                        // Dead key is already deactivated above
                     return true; // Let the original key be processed normally
                }
            }
    }
    return true;
}

#ifdef CAPS_WORD_ENABLE
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_MINS:
        case KY_QU:
        case KC_UNDS:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}
#endif 
/* N-SHOT MOD CONFIGURATION.
 * Reminder of the nshot structure:
 * 
 * typedef struct {
 *     uint16_t      trigger;          // Keycode to activate the n-shot mod
 *     uint8_t       modbit;           // Modbit allows for mod combos.
 *     uint8_t       alternate_modbit; // Alternate modbit for Gui-Ctl swapping.
 *     uint8_t       max_count;        // one-shot, two-shot, ..., n-shot.
 *     bool          active_on_rolls;  // Behavior for A down, Mod down, A up = (active_on_rolls == true ? Mod-A : a)
 *     oneshot_state state;            // Direct from users/callum
 *     uint8_t       count;            // N-shot count state
 *     bool          had_keydown;      // keydown state for (active_on_rolls == false)
 * } nshot_state_t;
 *
 * OSR_SFT is a base-layer thumbshift key. It is more stringent about rolls than the other nshots.
 * If you are reading my code for ideas, there is almost certainly no reason why you should want to
 * use a homerolled nshot for thumbshift instead of using QMK's defaults.
 * I am committing crimes against good sense and also reasonable layer state with this guy.
 * It is on purpose, but trust me: you do not have this usecase, and default is better for your usecase.
*/

#define MODBIT_GUICTL  MOD_BIT(KC_LCTL) | MOD_BIT(KC_LGUI) 
#define MODBIT_MEH     MOD_BIT(KC_LCTL)
//| MOD_BIT(KC_LALT) | MOD_BIT(KC_LSFT) 
nshot_state_t  nshot_states[] = {
//| trigger  | modbit            | swap-to          | max | roll into | State         | ## | timer | keydown? | //roll-in action |
//|----------|-------------------|------------------|-----|-----------|---------------|----|-------|----------|------------------|
    {OS_LSFT,  MOD_BIT(KC_LSFT),  MOD_BIT(KC_LSFT),   1,   true,       os_up_unqueued,  0,   0,     false},    // S-a
    {OS_LCTL,  MOD_BIT(KC_LCTL),  MOD_BIT(KC_LGUI),   1,   true,       os_up_unqueued,  0,   0,     false},    // C-a
    {OS_LALT,  MOD_BIT(KC_LALT),  MOD_BIT(KC_LALT),   1,   true,       os_up_unqueued,  0,   0,     false},    // A-a
    {OS_RALT,  MOD_BIT(KC_RALT),  MOD_BIT(KC_RALT),   1,   true,       os_up_unqueued,  0,   0,     false},    // AltGr-a
    {OS_LGUI,  MOD_BIT(KC_LGUI),  MOD_BIT(KC_LCTL),   1,   true,       os_up_unqueued,  0,   0,     false},    // G-a
    {OS_LGLC,  MODBIT_GUICTL   ,  MODBIT_GUICTL   ,   1,   true,       os_up_unqueued,  0,   0,     false},    // G-C-a
    {OS_LMEH,  MODBIT_MEH      ,  MODBIT_MEH      ,   1,   true,       os_up_unqueued,  0,   0,     true},    // C-S-A-a
    {TS_LCTL,  MOD_BIT(KC_LCTL),  MOD_BIT(KC_LCTL),   2,   true,       os_up_unqueued,  0,   0,     false},    // C-a
    {OSR_SFT,  MOD_BIT(KC_LSFT),  MOD_BIT(KC_LSFT),   1,   false,      os_up_unqueued,  0,   0,     false}     // a
};
uint8_t        NUM_NSHOT_STATES = sizeof(nshot_states) / sizeof(nshot_state_t);

bool is_nshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
        case PANIC:
        case CLEAR:
        case NAVMODE:
            return true;
        default:
            return false;
    }
}

bool is_nshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
        case NAVMODE:
        case NUM_OSL:
        case NUMMODE:
        case FUNMODE:
        case SYMMODE:
        case MCRMODE:
        case OS_LSFT:
        case OS_LCTL:
        case OS_LALT:
        case OS_LGUI:
            return true;
        default:
            return false;
    }
}

