/*
  Profile.h
  This file contains the functions for storing and editing the key mappings and profile images

  Images themselves are kept in Images.h
  Keymaps are stored in Keymaps.h

  Ideally, I would have used external flash storage for these so they could persist across flashings of code,
    along with being updated on the fly, but I'm apparently not that forward-thinking. Maybe next time.

*/

#ifndef PROFILE_H
#define PROFILE_H

#include "board.h"
#include <stdint.h>

// initializes pins, key arrays, and variables
void profile_init();
// switches to the passed in profile, updating key mappings and displaying new image
void profile_switch(uint8_t new_pro);
// reads pressed keys and sends keystrokes as needed
void profile_send_keys();
// reads and sends joystick values; reads pressed L3 and sends as necessary
void profile_send_joystick();

#endif
