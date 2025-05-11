#include "possumvibes.h"

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
static uint8_t df_layer = 0;

// Smart Layer config
bool _num_mode_active = false;
bool _func_mode_active = false;
bool _nav_mode_active = false;
bool _sym_mode_active = false;
bool _macro_mode_active = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
    if(record->event.pressed)
        uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif

    bool is_shifted =
        (get_mods() & MOD_MASK_SHIFT) ||
        (get_oneshot_mods() & MOD_MASK_SHIFT);

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
    }

    switch (keycode) {
        // Custom Tap/Holds
        case F3_TH: return override_th_hold(S(KC_F3), record);
        case F5_TH: return override_th_hold(S(KC_F5), record);
        case F6_TH: return override_th_hold(S(KC_F6), record);
        case F11_TH: return override_th_hold(S(KC_F11), record);
        case F12_TH: return override_th_hold(C(KC_F12), record);

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

        // Funky Symbol Shifts
        case KC_COMM: return override_shift(is_shifted, keycode, KC_EXLM, record);
        case KC_DOT:  return override_shift(is_shifted, keycode, KC_QUES, record);
        case KC_DQUO: return send_autopair_on_shift(is_shifted, KC_DQUO, KC_DQUO, keycode, record);
        case KC_GRV:  return send_autopair_on_shift(is_shifted, KC_GRV, KC_GRV, keycode, record);

        case KC_LPRN: return send_autopair_on_shift(is_shifted, KC_LPRN, KC_RPRN, keycode, record);
        case KC_RPRN: return send_string_c_function(is_shifted, keycode, record);
        case ANGLEBR: return send_autopair(KC_LABK, KC_RABK, record);
        case BRCKETS: return is_shifted 
            ? send_autopair(KC_LCBR, KC_RCBR, record) 
            : send_autopair(KC_LBRC, KC_RBRC, record);

        case KC_LABK: return send_double_on_shift(is_shifted, keycode, record);
        case KC_RABK: return send_double_on_shift(is_shifted, keycode, record);
        case KC_AMPR: return send_double_on_shift(is_shifted, keycode, record);
        case KC_ASTR: return send_double_on_shift(is_shifted, keycode, record);
        case KC_AT:   return send_double_on_shift(is_shifted, keycode, record);
        case KC_BSLS: return send_double_on_shift(is_shifted, keycode, record);
        case KC_CIRC: return send_double_on_shift(is_shifted, keycode, record);
        case KC_EQL:  return send_double_on_shift(is_shifted, keycode, record);
        case KC_HASH: return send_double_on_shift(is_shifted, keycode, record);
        case KC_MINS: return send_double_on_shift(is_shifted, keycode, record);
        case KC_PERC: return send_double_on_shift(is_shifted, keycode, record);
        case KC_PIPE: return send_double_on_shift(is_shifted, keycode, record);
        case KC_PLUS: return send_double_on_shift(is_shifted, keycode, record);
        case KC_QUES: return send_double_on_shift(is_shifted, keycode, record);
        case KC_SLSH: return send_double_on_shift(is_shifted, keycode, record);

        #ifdef CAPS_WORD_ENABLE
        case KC_DLR:
            if (record->event.pressed) {
                if (is_shifted) {
                    ensure_lowercase(is_shifted);

                    register_code16(KC_DLR);
                    caps_word_on();

                    return false;
                }
            }
            else {
                unregister_code16(KC_DLR);
            }

            return true;
        #endif
        case KC_TILD:
            if(record->event.pressed) {
                if (is_shifted) {
                    uint8_t mod_state = get_mods();
                    clear_shift(is_shifted);

                    SEND_STRING("~/");
                    set_mods(mod_state);
                    return false;
                }
            }
            return true;

        // Other
        case GET_SET: {
            if(record->event.pressed){
                ensure_lowercase(is_shifted);
                SEND_STRING("{ get; set; }");
            }
            return false;
        }

        case RPR_SCL: {
            if(record->event.pressed){
                uint8_t mod_state = get_mods();
                clear_shift(is_shifted);

                if(is_shifted){
                    tap_code16(KC_LPRN);
                }
                tap_code16(KC_RPRN);
                tap_code16(KC_SCLN);
                set_mods(mod_state);
            }
            return false;
        }

        case LMBD_FN:
            if(record->event.pressed){
                if(is_shifted){
                    uint8_t mod_state = get_mods();
                    clear_shift(is_shifted);
                    SEND_STRING("() =>");
                    set_mods(mod_state);
                } else {
                    SEND_STRING("=>");
                }
            }
            return false;
        
        // Shortcuts and macros
        
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
            layer_move(df_layer);

            // and caps lock/word
            clear_caps();
            return false;
        }

        case ISLINUX: {
            if(record->event.pressed){
                is_linux = !is_linux;

                df_layer = is_linux ? _APT : _DF_MAC;
                layer_move(df_layer);
                default_layer_set(df_layer);
            }
            return false;
        }
        case MACMODS:
            if (is_linux) return false;

            if (record->event.pressed){
                df_layer = _DF_MAC; 
                layer_move(df_layer);
                default_layer_set(df_layer);
            }
            return false;
        case LINMODS:
            if (is_linux) return false;

            if (record->event.pressed){
                df_layer = _APT; 
                layer_move(df_layer);
                default_layer_set(df_layer);
            }
            return false;

        case APT:
            if(record->event.pressed){
                df_layer = is_linux ? _APT: _DF_MAC;
                layer_move(df_layer);
                default_layer_set(df_layer);
            }
            return false;
        case QWERTY:
            if(record->event.pressed){
                df_layer = is_linux ? _QWERTY: _ALT_MAC;
                layer_move(df_layer);
                default_layer_set(df_layer);
            }
            return false;

        case GOTO_1: return send_alternate_key(G(KC_1), C(KC_1), !is_linux, record);
        case GOTO_2: return send_alternate_key(G(KC_2), C(KC_2), !is_linux, record);
        case GOTO_3: return send_alternate_key(G(KC_3), C(KC_3), !is_linux, record);
        case GOTO_4: return send_alternate_key(G(KC_4), C(KC_4), !is_linux, record);
        case GOTO_5: return send_alternate_key(G(KC_5), C(KC_5), !is_linux, record);

        case LOCKSCR:  return send_alternate_key(C(A(KC_L)), G(C(KC_Q)), !is_linux, record);
        case UND_RED: 
            if (is_linux){
                return override_shift(is_shifted, C(KC_Z), C(KC_Y), record);
            }
            return override_shift(is_shifted, G(KC_Z), G(KC_Y), record);


        case DLAYOUT:
            if (is_linux){
                if(record->event.pressed){
                    tap_code16(G(A(KC_SPC)));
                }
                return true;
            }

            if (record->event.pressed){
                tap_code16(G(S(KC_LBRC)));
            }
            return false;

        case PRVDESK:  return send_alternate_key(G(KC_LEFT), C(KC_LEFT), !is_linux, record);
        case NXTDESK:  return send_alternate_key(G(KC_RGHT), C(KC_RGHT), !is_linux, record);

        case S_CUT:    return send_alternate_key(S(KC_DEL),  G(KC_X),    !is_linux, record);
        case S_COPY:   return send_alternate_key(C(KC_INS),  G(KC_C),    !is_linux, record);
        case S_PASTE:  return send_alternate_key(S(KC_INS),  G(KC_V),    !is_linux, record);
        case S_BACK:   return send_alternate_key(A(KC_LEFT), G(KC_LBRC), !is_linux, record);
        case S_FWD:    return send_alternate_key(A(KC_RGHT), G(KC_RBRC), !is_linux, record);
        case S_CLOSE:  return send_alternate_key(C(KC_W),    G(KC_W),    !is_linux, record);
        case S_SAVE:   return send_alternate_key(C(KC_S),    G(KC_S),    !is_linux, record);

        case KILLWRD:
            if (get_mods() & MOD_MASK_ALT){
                return send_alternate_key(C(KC_DEL), A(KC_DEL), !is_linux, record);
            }
            return send_alternate_key(C(KC_BSPC), A(KC_BSPC), !is_linux, record);

        case KC_HOME:
            // Linux is normal. Mac is horrible.
            if(is_linux || (get_mods() & MOD_MASK_CTRL)){
                return true;
            }

            if (record->event.pressed){
                  tap_code16(C(KC_A));
            }
            return false;
        case KC_END:
            // Linux is normal. Mac is horrible.
            if(is_linux || (get_mods() & MOD_MASK_CTRL)){
                return true;
            }

            if (record->event.pressed){
                  tap_code16(C(KC_E));
            }
            return false;
        
        case MD_LINK: return send_string_markdown_link(record);

        case MD_CODE:
            if(record ->event.pressed) {
                // clear shift temporarily
                uint8_t mod_state = get_mods();
                clear_shift(is_shifted);

                // ``` ```|
                SEND_STRING("``` ```");

                // ```| ```
                triple_tap(KC_LEFT);
                tap_code(KC_LEFT);

                // restore previous shift state
                set_mods(mod_state);
            }
            return false;
        case QMKCOMP: {
            if(record->event.pressed){
                clear_caps();
                SEND_STRING("qmk compile\n");
            }
            return false;
        }
        case QMKFLSH: {
            if(record->event.pressed){
                clear_caps();
                SEND_STRING("qmk flash\n");
            }
            return false;
        }
        case KY_V1: return send_string_version(is_shifted, KC_1, record);
        case KY_V2: return send_string_version(is_shifted, KC_2, record);
        case KY_V3: return send_string_version(is_shifted, KC_3, record);

        case KY_QU:
            if(record->event.pressed){
                if(is_caps_word_on()){
                    // tapping keys to keep caps word enabled; note that KY_QU must be a continue-uncapitalized key for caps_word
                    tap_code16(S(KC_Q));
                    tap_code16(S(KC_U));
                    return false;
                }

                tap_code(KC_Q);

                uint8_t mod_state = get_mods();
                clear_shift(is_shifted);

                tap_code(KC_U);
                set_mods(mod_state);
            }
            return false;

        case VI_ZZ:
            if (record ->event.pressed) {
              ensure_lowercase(is_shifted);
              SEND_STRING("\e:wq\n");
            }
            return false;
        case VI_ZQ:
            if(record->event.pressed){
                ensure_lowercase(is_shifted);
                SEND_STRING("\e:q!\n");
            }
            return false;

        case VI_AW:  return send_string_vi_yiw(is_shifted, false, KC_A, true, record);
        case VI_IW:  return send_string_vi_yiw(is_shifted, false, KC_I, true, record);
        case VI_YAW: return send_string_vi_yiw(is_shifted, true, KC_A, true, record);
        case VI_YIW: return send_string_vi_yiw(is_shifted, true, KC_I, true, record);

        case VI_YI:
          if (record->event.pressed){
                uint8_t code = is_shifted ? KC_A : KC_I;
                return send_string_vi_yiw(is_shifted, true, code, false, record);
            }
          return false;
    }

    return true;
};

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
nshot_state_t  nshot_states[] = {
//| trigger  | modbit            | swap-to          | max | roll into | State         | ## | timer | keydown? | //roll-in action |
//|----------|-------------------|------------------|-----|-----------|---------------|----|-------|----------|------------------|
    {OS_LSFT,  MOD_BIT(KC_LSFT),  MOD_BIT(KC_LSFT),   1,   true,       os_up_unqueued,  0,   0,     false},    // S-a
    {OS_LCTL,  MOD_BIT(KC_LCTL),  MOD_BIT(KC_LGUI),   1,   true,       os_up_unqueued,  0,   0,     false},    // C-a
    {OS_LALT,  MOD_BIT(KC_LALT),  MOD_BIT(KC_LALT),   1,   true,       os_up_unqueued,  0,   0,     false},    // A-a
    {OS_RALT,  MOD_BIT(KC_RALT),  MOD_BIT(KC_RALT),   1,   true,       os_up_unqueued,  0,   0,     false},    // AltGr-a
    {OS_LGUI,  MOD_BIT(KC_LGUI),  MOD_BIT(KC_LCTL),   1,   true,       os_up_unqueued,  0,   0,     false},    // G-a
    {OS_LGLC,  MODBIT_GUICTL,     MODBIT_GUICTL,      1,   true,       os_up_unqueued,  0,   0,     false},    // G-C-a
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

