#include "board.h"
#include "Profile.h"

#include <stdint.h>
#include <Arduino.h>
#include <Keyboard.h>

typedef enum{pressed, not_pressed} key_state_t;

// key definition
typedef struct key
{
  uint8_t row;
  uint8_t col;
  key_state_t state;

} key_t;

// variables to hold the current profile's data
static uint8_t profile_num = 0;

extern const uint8_t row_pins[];
extern const uint8_t col_pins[];

static key_t keys[NUM_ROWS][NUM_COLS];

void profile_init()
{
  // Set pin modes as necessary
  for (int i = 0; i < NUM_ROWS; i++)
  {
    pinMode(row_pins[i], INPUT_PULLDOWN);
  }
  for (int i = 0; i < NUM_COLS; i++)
  { 
    digitalWrite(col_pins[i], LOW);
    pinMode(col_pins[i], OUTPUT);
  }
  // TODO: OLED pin setup if necessary

  // Create array of key states
  for(uint8_t r = 0; r < NUM_ROWS; r++)
  {
    for(uint8_t c = 0; c < NUM_COLS; c++)
    {
      keys[r][c].row = row_pins[r];
      keys[r][c].col = col_pins[c];
    }
  }
  // Switch to default profile
  profile_switch(0);
}

// This function moves the saved keymaps
void profile_switch(uint8_t new_pro)
{
  profile_num = new_pro;
  
}
