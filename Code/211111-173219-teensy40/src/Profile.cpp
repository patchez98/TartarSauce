#include "platformIncludes.h"
#include "board.h"
#include "TartarSauce.h"
#include "Profile.h"
#include "Keymaps.h"

#include <stdint.h>
#include <Arduino.h>

// Keyboard implementation-dependent declarations
#ifdef BOARD_RP2040

// Adafruit TinyUSB required definitions
enum e_report_ids {JOYSTICK_REPORT_ID = 1, KEYBOARD_REPORT_ID = 2};
uint8_t const desc_hid_report[] =
{
  TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(JOYSTICK_REPORT_ID)), // First gamepad report-id 1
  TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(KEYBOARD_REPORT_ID))
};

Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);

// Report payload defined in src/class/hid/hid.h
// - For Gamepad Button Bit Mask see  hid_gamepad_button_bm_t
// - For Gamepad Hat    Bit Mask see  hid_gamepad_hat_t
hid_gamepad_report_t gamepad;

// Keyboard globals
static uint8_t pressed_count = 0; // How many (non-modifier) keys are pressed right now. Up to 6 are available, which should be plenty for a macropad such as this.
static uint8_t keycode[6] = {0}; // Holds up to 6 pressed keys
static bool key_update_required = false; // Whether or not we should send an update packet

#elif BOARD_TEENSY40
#endif


// variables to hold the current profile's data
static uint8_t profile_num = 0;

extern const uint8_t row_pins[];
extern const uint8_t col_pins[];


// Keyswitch matrix
static keyswitch_t keys[NUM_ROWS][NUM_COLS];
static uint8_t mod_keys = 0x00; // Holds state of shift, ctrl, etc.; only used in some keyboard implementations
// Values for joystick input
static int16_t joy_y;
static int16_t joy_x;
static keyswitch_t L3;
static joy_type_t joy_type = analog;
static joy_analog_t joy_analog = {
  .deadzone = (uint16_t)((float)(1<<(ADC_RESOLUTION-1)) * 0.2f),
  .max_x = 1<<(ADC_RESOLUTION-1),
  .min_x = 0,
  .max_y = 1<<(ADC_RESOLUTION-1),
  .min_y = 0,
};

// -- Private function declarations --
static void key_pressed(keyswitch_t * k);
static void key_released(keyswitch_t * k);
static void key_send();
static void joy_analog_update();
static void joy_send();

// -- Public function Definitions --


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
  analogReadResolution(ADC_RESOLUTION);
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(L3_PIN, INPUT_PULLUP);
  pinMode(JOY_DETECT_PIN, INPUT_PULLUP);
  // TODO: OLED pin setup if necessary

  // Initialize L3 key (row and column irrelevant)
  L3.row = 0;
  L3.col = 0;
  L3.state = not_pressed;
  L3.map = HID_KEY_SPACE; // will be overwritten upon profile switch
  
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

  // Initialize joystick (board-dependent)
#ifdef BOARD_RP2040
  TinyUSB_Device_Init(0);
  usb_hid.begin();
  // wait until device mounted
  while( !TinyUSBDevice.mounted() ) delay(1);

#elif BOARD_TEENSY40
  Joystick.useManualSend(true);
#endif

  // Switch to default profile
  profile_switch(0);
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
      if(digitalRead(row_pins[r]) == HIGH)
      {
        // Switch pressed since last scan
        key_pressed(&keys[r][c]);
      }
      // Check for rising edge
      else if(digitalRead(row_pins[r]) == LOW)
      {
        // Switch released since last scan
        key_released(&keys[r][c]);
      }
    }
    // Return column to low
    digitalWrite(col_pins[c], LOW);
    // short delay to allow pin voltage to fall
    delayMicroseconds(5);
  }

  // Send HID message
  key_send();
}
static uint32_t numNotReady = 0;

void profile_send_joystick()
{
  // Read and update analog values
  joy_analog_update();

  // L3
  // Check for falling edge
  if(digitalRead(L3_PIN) == LOW)
  {
    // Switch pressed since last scan
    key_pressed(&L3);
  }
  // Check for rising edge
  else if(digitalRead(L3_PIN) == HIGH)
  {
    // Switch released since last scan
    key_released(&L3);
  }

  joy_send();
}

bool profile_is_usb_ready()
{
  return usb_hid.ready();
}


void Profile_DebugPrint()
{
  
#ifdef DEBUG
  if(SERIAL_PORT)
  {
    SERIAL_PORT.print("\n\r");
    SERIAL_PORT.printf("Y: %5d\n\r", joy_y);
    SERIAL_PORT.printf("X: %5d\n\r", joy_x);
    SERIAL_PORT.printf("L3: %c\n\r", (L3.state == pressed ? 'T' : 'F'));
    SERIAL_PORT.printf("L3Raw: %c\n\r", (digitalRead(L3_PIN) == LOW ? 'T' : 'F'));
    SERIAL_PORT.printf("NotReady: %5u\n\r", numNotReady);
  }
#endif // DEBUG
}

// -- Private function Definitions --

// Keyboard implementation-dependent functions
static void key_pressed(keyswitch_t * k)
{
#ifdef BOARD_RP2040
  // Check for modifier keys
  if((k->map & KEY_MODIFIER_MASK) == KEY_MODIFIER_MASK)
  {
    // Modifier key
    mod_keys |= (k->map & 0xFF);
    key_update_required = true;
  }
  else if(pressed_count < 6)
  {
    // Not a modifier key; add it to the packet if we have room
    keycode[pressed_count++] = k->map;
    key_update_required = true;
  }
#elif BOARD_TEENSY40
  if(k->state == not_pressed)
  {
    Keyboard.press(k->map);
  }
#endif
  k->state = pressed;
}

static void key_released(keyswitch_t * k)
{
#ifdef BOARD_RP2040
  // Check for modifier keys
  if(k->map & 0x100)
  {
    // Modifier key
    mod_keys &= ~(k->map & 0xFF);
    key_update_required = true;
  }
  else
  {
    // Not a modifier key; mark that we need to send an update packet
    key_update_required = true;
  }
#elif BOARD_TEENSY40
  // We have to manually release each key since the Teensy implementation remembers
  if(k->state == pressed)
  {
    Keyboard.release(k->map);
  }
#endif
  k->state = not_pressed;
}

// Sends key packet and resets for the next round
static void key_send()
{
#ifdef BOARD_RP2040
  // Check to see if we actually need to update / are able to send an update now
  if(key_update_required && usb_hid.ready())
  {
    // We need to update and the HID system is ready
    usb_hid.keyboardReport(KEYBOARD_REPORT_ID, mod_keys, keycode);
  }
  else if(!usb_hid.ready())
  {
    numNotReady++;
  }
  
  // Clear and reset for next send, regardless of whether or not this send was successful
  key_update_required = false; // reset send flag
  pressed_count = 0; // Reset pressed keys counter
  memset(keycode, 0, sizeof keycode); // clear out keycode list
  // We don't reset modifier keys here; that's done in the key released function
#elif BOARD_TEENSY40
#endif
}

static int joy_xMin = 1<<(ADC_RESOLUTION-1);
static int joy_xMax = 1<<(ADC_RESOLUTION-1);
static int joy_yMin = 1<<(ADC_RESOLUTION-1);
static int joy_yMax = 1<<(ADC_RESOLUTION-1);
static int joy_xCenter = 0;
static int joy_yCenter = 0;

static void joy_analog_update()
{
  // Determine joystick type
  joy_type = (digitalRead(JOY_DETECT_PIN) ? analog : dPad);
  switch(joy_type)
  {
    case dPad:
    {

      break;
    }
    case analog:
    {
      static bool centerSet = false;
      //joy_x = random(0, 1<<ADC_RESOLUTION);
      //joy_y = random(0, 1<<ADC_RESOLUTION);
      joy_x = analogRead(X_PIN);
      joy_y = analogRead(Y_PIN);
      //if(!centerSet)
      if(digitalRead(SW0_PIN) == false)
      {
        centerSet = true;
        joy_xCenter = joy_x - (1<<(ADC_RESOLUTION-1));
        joy_yCenter = joy_y - (1<<(ADC_RESOLUTION-1));
      }
      //joy_x = ((joy_x - 0) * (127 - (-128)) / ((1<<ADC_RESOLUTION) - 0)) + (-128);
      if(joy_x < joy_xMin)
      {
        joy_xMin = joy_x;
      }
      if(joy_x > joy_xMax)
      {
        joy_xMax = joy_x;
      }
      if(joy_y < joy_yMin)
      {
        joy_yMin = joy_y;
      }
      if(joy_y > joy_yMax)
      {
        joy_yMax = joy_y;
      }
      joy_x += joy_xCenter;
      joy_y += joy_yCenter;

#ifdef BOARD_RP2040
      //gamepad.x       = joy_x;
      //gamepad.y       = joy_y;
      gamepad.x       = map(joy_x, joy_xMin, joy_xMax, -128, 127);
      gamepad.y       = map(joy_y, joy_yMin, joy_yMax, -128, 127);
      gamepad.z       = 0;
      gamepad.rz      = 0;
      gamepad.ry      = 0;
      gamepad.hat     = 0;
      gamepad.buttons = 0;
#elif BOARD_TEENSY40
      Joystick.X(joy_x);
      Joystick.Y(joy_y);
#endif
      break;
    }
  }

}

static void joy_send()
{
#ifdef BOARD_RP2040
  if(usb_hid.ready())
  {
    usb_hid.sendReport(JOYSTICK_REPORT_ID, &gamepad, sizeof(gamepad));
  }
  else
  {
    numNotReady++;
  }
#elif BOARD_TEENSY40
  Joystick.send_now()
#endif
}
