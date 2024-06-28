#ifndef KEYMAPS_H
#define KEYMAP_H

#include <stdint.h>
#include "platformIncludes.h"
#include "board.h"

// Each list of keymaps is a 2d array that should appear the same as the physical layout
// Unused / nonexistent keys should have a value of 0
// Empty keymap available at end of file
// NOTE: Ensure there is a value for the joystick L3 as well
const uint16_t matrix_keymaps[][NUM_ROWS][NUM_COLS] = 
{
  // Profile 1 (Bee movie Script)
  {
    {KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, 0},
    {KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_Q},
    {KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P}
  },
  // Profile 2 (Overwatch)
  {
    {KEY_ESC, KEY_O, KEY_C, KEY_I, KEY_P, 0},
    {KEY_TAB, KEY_H, KEY_TILDE, KEY_Q, KEY_R, KEY_S},
    {KEY_LEFT_CTRL, KEY_LEFT_SHIFT, KEY_V, KEY_SPACE, KEY_E, KEY_F}
  },
  // Profile 3 (JRPG)
  {
    {KEY_E, KEY_E, KEY_E, KEY_E, KEY_E, 0},
    {KEY_E, KEY_E, KEY_E, KEY_E, KEY_E, KEY_E},
    {KEY_E, KEY_E, KEY_E, KEY_E, KEY_E, KEY_E}
  },
  // Profile 4 (Test)
  {
    {KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, 0},
    {KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_ESC},
    {KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N}
  }
};

// Joystick L3 keymaps
// Can be keyswitches like above, not joystick buttons
const uint16_t L3_keymaps[] = 
{
  // Profile 1 (Bee movie script)
  KEY_SEMICOLON,
  // Profile 2 (Overwatch)
  KEY_LEFT_SHIFT,
  // Profile 3 (JRPG)
  KEY_LEFT_SHIFT,
  // Profile 4 (Test)
  KEY_LEFT_SHIFT
};

/* Blank profile to copy over
// NOTE: May need keys added/replaced depending on board version
// Profile  ()
{
  {KEY_, KEY_, KEY_, KEY_, KEY_, 0},
  {KEY_, KEY_, KEY_, KEY_, KEY_, KEY_},
  {KEY_, KEY_, KEY_, KEY_, KEY_, KEY_}
}
*/

#endif