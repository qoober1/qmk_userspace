#include "possumvibes.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_APT]       = LAYOUT_wrapper(HRM(LAYER_APTv3))

    ,[_ALPHA2]   = LAYOUT_wrapper(LAYER_ALPHA2)

    // ,[_DF_MAC]   = LAYOUT_wrapper(MAC_HRM(LAYER_APTv3))
    // ,[_QWERTY]   = LAYOUT_wrapper(HRM(LAYER_APTv3))
    // ,[_ALT_MAC]  = LAYOUT_wrapper(MAC_HRM(LAYER_APTv3))

    ,[_COMBOREF] = LAYOUT_wrapper(LAYER_COMBOREF)

    ,[_FUNC]     = LAYOUT_wrapper(LAYER_FUNC)

    ,[_MACRO]    = LAYOUT_wrapper(LAYER_MACRO)

    ,[_NUM]      = LAYOUT_wrapper(HRM(LAYER_NUM))

    ,[_SYM]      = LAYOUT_wrapper(LAYER_SYM)

    ,[_NAV]      = LAYOUT_wrapper(LAYER_NAV)

    ,[_SYSTEM]   = LAYOUT_wrapper(LAYER_SYSTEM)

};

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    return true;
};
