#pragma once

// Tapping settings
#define TAPPING_TERM 200                 // Base tapping term
#define QUICK_TAP_TERM TAPPING_TERM / 2  // Quick Tap Term to trigger auto-repeat

// Allows media codes to properly register in macros and rotary encoder code
#define TAP_CODE_DELAY 15               // Introduces delay between code taps so no keys get eaten.

// Combos!
#define COMBO_TERM 36                   // Gotta combo fast.
#define COMBO_ONLY_FROM_LAYER 2         // compare only with the COMBOREF layer.
// #define COMBO_TERM_PER_COMBO            // Adjust combo term for ease of combo.
// #define COMBO_STRICT_TIMER              // all keys must be hit within the term from the first keypress.
#define EXTRA_SHORT_COMBOS

// EE hands will literally always be enabled
#undef EE_HANDS
#define EE_HANDS

// debounce getting  funky
#define DEBOUNCE 10

// SPACE SAVERS FOR ALL BOARDS.
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE
#define LAYER_STATE_16BIT

// Layout macros
#ifndef __ASSEMBLER__
#   include "layout.h"
#endif
