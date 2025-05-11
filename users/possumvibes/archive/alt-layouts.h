/* alt layouts: not in use */
// APTHD. APTv3 variant with reduced load on pinkies. Not in use--put too much load on index for me
#define LAYER_APTHD \
    KC_W,    KC_C,    KH_Y,    KC_D,    KC_B,    KC_Q,    KC_L,    KH_U,    KC_O,    KC_QUOT, \
    KH_R,    KH_S,    KH_T,    KH_H,    KC_K,    KC_J,    KH_N,    KH_E,    KH_A,    KH_I,    \
    KC_X,    KC_F,    KC_G,    KC_P,    KC_V,    KC_Z,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, \
                               THM_LH0, THM_LH1, THM_RH1, THM_RH0

// AptMak
#define LAYER_APTMAK \
    KC_Q,    KC_W,    KH_F,    KC_P,    KC_B,    KC_Z,    KC_L,    KH_U,    KC_Y,    KC_QUOT, \
    KH_R,    KH_S,    KH_T,    KH_H,    KC_K,    KC_J,    KH_N,    KM_A,    KS_I,    KC_O,    \
    KC_X,    KC_C,    KC_G,    KC_D,    KC_V,    XXXXXXX, KC_M,    KC_COMM, KC_DOT,  XXXXXXX, \
                               KC_SPC,  NAVMOD,  OS_LSFT, KC_EE

// APTv5: my first 30 key layout, intended for v on combo. Turns out I don't care for that,
// so V is back on pinky for the moment so it's available when using this layout.
#define LAYER_APTv5 \
    KC_W,    KC_C,    KC_G,    KC_D,    KC_QUOT, KC_X,    KC_L,    KC_U,    KC_O,    KC_Y,    \
    KC_R,    KC_S,    KC_T,    KC_H,    KC_K,    KC_J,    KC_N,    KC_E,    KC_A,    KC_I,    \
    KC_V,    KC_B,    KC_P,    KC_F,    KC_Q,    KC_Z,    KC_M,    KC_COMM, KC_DOT,  XXXXXXX, \
                               THM_LH0, THM_LH1, THM_RH1, THM_RH0


// Canary
#define LAYER_CANARY \
    KC_W,    KC_L,    KH_Y,    KC_P,    KC_B,    KC_Z,    KC_F,    KS_O,    KC_U,    KC_QUOT, \
    KS_C,    KS_R,    KS_S,    KS_T,    KC_G,    KC_M,    KH_N,    KH_E,    KS_I,    KY_A,    \
    KC_Q,    KC_J,    KC_V,    KC_D,    KC_K,    KC_X,    KC_H,    KC_SCLN,  KC_COMM, KC_DOT, \
                               THM_LH0, THM_LH1, THM_RH1, THM_RH0
