#include "possumvibes.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_APT] = LAYOUT_34key_w(HRM(LAYER_APTMAKNT))
    ,[_DF_MAC] = LAYOUT_34key_w(MAC_HRM(LAYER_APTMAKNT))

    ,[_QWERTY] = LAYOUT_34key_w(HRM(LAYER_APTv3))
    ,[_ALT_MAC] = LAYOUT_34key_w(MAC_HRM(LAYER_APTv3))

    ,[_COMBOREF] = LAYOUT_34key_w(LAYER_COMBOREF)

    ,[_FUNC] = LAYOUT_34key_w(LAYER_FUNC)

    ,[_MACRO] = LAYOUT_34key_w(LAYER_MACRO)

    ,[_NUM] = LAYOUT_34key_w(HRM(LAYER_NUM))

    ,[_SYM] = LAYOUT_34key_w(LAYER_SYM)

    ,[_NETHACK] = LAYOUT_34key_w(HRM(LAYER_NETHACK))

    ,[_NAV] = LAYOUT_34key_w(LAYER_NAV)

    ,[_SYSTEM] = LAYOUT_34key_w(LAYER_SYSTEM)

};

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    return true;
};
