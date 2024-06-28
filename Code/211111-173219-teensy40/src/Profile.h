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
#include "platformIncludes.h"
#include <stdint.h>

typedef enum{pressed, not_pressed} key_state_t;

// key definition
typedef struct keyswitch
{
  uint8_t row;
  uint8_t col;
  key_state_t state;
  uint16_t map;

} keyswitch_t;

typedef enum {dPad = 0, analog = 1} joy_type_t;

typedef struct {
  uint16_t deadzone;
  uint16_t max_x;
  uint16_t min_x;
  uint16_t max_y;
  uint16_t min_y;
} joy_analog_t;

typedef struct {
  keyswitch_t up_map;
  keyswitch_t dn_map;
  keyswitch_t lt_map;
  keyswitch_t rt_map;
} joy_dPad_t;

// initializes pins, key arrays, and variables
void profile_init();
// switches to the passed in profile, updating key mappings and displaying new image
void profile_switch(uint8_t new_pro);
// reads pressed keys and sends keystrokes as needed
void profile_send_keys();
// reads and sends joystick values; reads pressed L3 and sends as necessary
void profile_send_joystick();
// waits until the USB HID is ready for another packet (RP2040 only)
bool profile_is_usb_ready();
// Outputs current pressed buttons to terminal
void Profile_DebugPrint();

#endif
