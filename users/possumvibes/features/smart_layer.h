#pragma once
#include QMK_KEYBOARD_H

/* SMART LAYERS
 * These handy creations use custom functions per layer/group of identically-behaving layers
 * to automatically control layer exit. This means the layer can be tapped into and will
 * automatically turn itself off! The easiest usecase to understand is a smart num layer,
 * that allows you to type numbers like on a toggled layer, but hit space and be back to alpha
 * on the next keystroke.
 * 
 * I use these for a lot of layers, and I use them to create a oneshot layer that ignores
 * mods toward the oneshot count! It's great for me.
 * 
 * To replicate these outside my code,
   *  use the basic contracts defined in each section,
   * put a static bool for each smart layer in your keymap.c/<USERNAME>.c:
     *  static bool _<NAME>_mode_active = false;
   * and use num_mode_enable and num_mode_process as your starting points.
   * 
 * three cheers for not holding down layer keys!
*/



/* -------- Default Smart Layers --------
 * These functions are for generic behaviorgeneric behavior * This is for mod-order-agnostic oneshot layers!
 * These will ignore nshot mods and disable the layer after one tap for everything else.
 * There's just the one method here!
*/

bool smart_layer_enable(uint8_t layer);
bool smart_layer_disable(uint8_t layer);

bool smart_oneshot_process(uint16_t keycode, keyrecord_t *record, uint8_t layer);

/* -------- Custom Smart Layers --------
 * Each of these layers has fancy logic for when the layer should be disabled.
 * Requirements:
   * <NAME>_mode_process(uint16_t keycode, keyrecord_t *record)
 * 
*/
bool num_mode_process(uint16_t keycode, keyrecord_t *record);

bool func_mode_process(uint16_t keycode, keyrecord_t *record);

bool nav_mode_enable(keyrecord_t *record);
bool nav_mode_process(uint16_t keycode, keyrecord_t *record);


