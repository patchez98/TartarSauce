#include "board.h"
#include "Profile.h"
#include "Keymaps.h"

#include <stdint.h>
#include <Arduino.h>
#include <usb_keyboard.h>



typedef enum{pressed, not_pressed} key_state_t;

// key definition
typedef struct keyswitch
{
  uint8_t row;
  uint8_t col;
  key_state_t state;
  uint16_t map;

} keyswitch_t;

// variables to hold the current profile's data
static uint8_t profile_num = 0;

extern const uint8_t row_pins[];
extern const uint8_t col_pins[];

// Keyswitch matrix
static keyswitch_t keys[NUM_ROWS][NUM_COLS];
// Values for analog stick input
static uint16_t vert;
static uint16_t hori;
static keyswitch_t L3;

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
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(L3_PIN, INPUT_PULLUP);
  // TODO: OLED pin setup if necessary

  // Initialize L3 key (row and column irrelevant)
  L3.row = 0;
  L3.col = 0;
  L3.state = not_pressed;
  // Create array of key states
  for(uint8_t r = 0; r < NUM_ROWS; r++)
  {
    for(uint8_t c = 0; c < NUM_COLS; c++)
    {
      keys[r][c].row = row_pins[r];
      keys[r][c].col = col_pins[c];
      keys[r][c].state = not_pressed;
    }
  }
  // Switch to default profile
  profile_switch(1);
}

// This function changes the keymap to that of one of the saved profiles
void profile_switch(uint8_t new_pro)
{
  profile_num = new_pro;
  // TODO: OLED display change
  
  // Assign new keymappings
  // Joystick map
  L3.map = L3_keymaps[profile_num];
  // Key matrix maps
  for(uint8_t r = 0; r < NUM_ROWS; r++)
  {
    for(uint8_t c = 0; c < NUM_COLS; c++)
    {
      keys[r][c].row = row_pins[r];
      keys[r][c].col = col_pins[c];
      keys[r][c].state = not_pressed;
      keys[r][c].map = matrix_keymaps[profile_num][r][c];
    }
  }
}

// This function reads which keys are pressed and sends appropriate messages to PC
void profile_send_keys()
{
  for(uint8_t c = 0; c < NUM_COLS; c++)
  {
    // Output high on a column
    digitalWrite(col_pins[c], HIGH);
    // Read and send keys if they are pressed
    for(uint8_t r = 0; r < NUM_ROWS; r++)
    {
      // Check for falling edge
      if(digitalRead(row_pins[r]) == HIGH && keys[r][c].state == not_pressed)
      {
        // Switch pressed since last scan
        keys[r][c].state = pressed;
        Keyboard.press(keys[r][c].map);
      }
      // Check for rising edge
      else if(digitalRead(row_pins[r]) == LOW && keys[r][c].state == pressed)
      {
        // Switch released since last scan
        keys[r][c].state = not_pressed;
        Keyboard.release(keys[r][c].map);
      }
    }
    // Return column to low
    digitalWrite(col_pins[c], LOW);
    // short delay to allow pin voltage to fall
    delayMicroseconds(5);
  }
}

void profile_send_joystick()
{
  //Read and update analog values
  //Vertical (Y) axis
  vert = analogRead(Y_PIN);
  Joystick.Y(vert);
  //Horizontal (X) axis
  hori = analogRead(X_PIN);
  Joystick.X(hori);
  //L3
  // Check for falling edge
  if(digitalRead(L3_PIN) == LOW && L3.state == not_pressed)
  {
    // Switch pressed since last scan
    L3.state = pressed;
    Keyboard.press(L3.map);
  }
  // Check for rising edge
  else if(digitalRead(L3_PIN) == HIGH && L3.state == pressed)
  {
    // Switch released since last scan
    L3.state = not_pressed;
    Keyboard.release(L3.map);
  }
}