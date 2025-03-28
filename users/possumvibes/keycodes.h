#pragma once
#include "quantum/keycodes.h"

enum layers { 
  _APT = 0,
  _DF_MAC,
  _COMBOREF,
  _QWERTY,
  _ALT_MAC,
  _FUNC,
  _NUM,
  _MACRO,
  _SYM,
  _NAV,
  _SYSTEM 
};

enum custom_keycodes {
    // OS-specific utilities
    ISLINUX =  QK_USER, 
    MACMODS, LINMODS,
    LOCKSCR, DLAYOUT, S_CUT, S_COPY, S_PASTE,  S_CLOSE, S_SAVE, KILLWRD,

    // Alpha layers
    APT, QWERTY, 

    // Layer modes and how to escape them
    NUMMODE, FUNMODE, NAVMODE, SYMMODE, MCRMODE, PANIC,

    // "Paired" macros: Undo/Redo, QMK Compile/Flash
    UND_RED, QMKCOMP, QMKFLSH,
    PRVDESK, NXTDESK, S_BACK, S_FWD,

    // Desktop nav
    GOTO_1, GOTO_2, GOTO_3, GOTO_4, GOTO_5, 

    // Custom markup and programming syntax
    ANGLEBR, BRCKETS, // self-closing brackets: <|>, [|]  {|}
    MD_LINK, MD_CODE, // Markdown: [links|]() and ```codeblocks|  ```
    GET_SET,  // { get; set; }
    RPR_SCL,  // ); ();
    LMBD_FN,  // () =>

    // Quick "not typing all that" strings
    KY_QU, KY_V1, KY_V2, KY_V3,

    // Vim macros: exactly what it says on the tin (words shiftable to Words)
    VI_ZZ, VI_ZQ,   VI_YAW, VI_YIW,   VI_YA, VI_YI,   VI_AW, VI_IW,

    // Swapper keys: Windows, tabs, and generic Reverse
    SW_WIN, SW_TAB, SW_REV,

    // Locking Mods and how to escape them
    ML_LCTL, ML_LSFT, ML_LALT, ML_LGUI, CLEAR, 

    // N-Shot Mods: "normal" callum-style n-shot mods (now featuring MULTI MODS!!!)
    OS_LSFT, OS_LCTL, OS_LALT, OS_RALT, OS_LGUI, OS_LGLC, TS_LCTL,
    OSR_SFT, // and also n-shot Thumbshift (see n-shot mods for rolling differences)

    NEW_SAFE_RANGE  // Use for keymap-specific codes
};

/* ---------- Aliases ---------- */

// oneshot layers
#define NUM_OSL     OSL(_NUM)
#define SYS_OSL     OSL(_SYSTEM)

// mouse button aliases for ease of use
#define LCLICK      KC_BTN1
#define RCLICK      KC_BTN2
#define MCLICK      KC_BTN3

// Shortcuts
#define S_KILL      G(KC_Q)

#define TMUXPRE     C(KC_SPC)
#define DMENU       G(KC_SPC)

// Macros
#define TH(k)       LT(0,k)
#define F3_TH       TH(KC_F3)
#define F5_TH       TH(KC_F5)
#define F6_TH       TH(KC_F6)
#define F11_TH      TH(KC_F11)
#define F12_TH      TH(KC_F12)

