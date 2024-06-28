#include <Arduino.h>
#include "board.h"
#include "Profile.h"
#include "platformIncludes.h"

// NOTE: Must ensure there are at least as many images in Images.h as profiles
#define NUM_PROFILES 4
// Used for setting polling rate
elapsedMicros poll_timer = 0;

#ifdef DEBUG
elapsedMillis debug_timer = 0;
#endif // DEBUG

void setup() 
{
  //delay(2000);
  
  // Pin initialization
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  // Turn on LEDs
  analogWrite(LED_PIN, PWM_DUTY * 256);

  // Initialize general purpose switches
  pinMode(SW0_PIN, INPUT_PULLUP);
  pinMode(SW1_PIN, INPUT_PULLUP);

  // Initialize keymap and LED
  profile_init();
#ifdef DEBUG
  // Test for PROGMEM stuff
  SERIAL_PORT.begin(115200);
  while(!SERIAL_PORT);
#endif // DEBUG
}

static uint32_t time0 = 0;
static uint32_t time1 = 0;
void loop() 
{
  if(poll_timer >= POLLING_US)
  {
    time0 = poll_timer;
    // Check joystick and send values
    profile_send_joystick();
    poll_timer = 0;
    while(!profile_is_usb_ready());
    //delayMicroseconds(1500);
    // Check keyswitches and send values as necessary
    profile_send_keys();
    //while(!profile_is_usb_ready());
    //delayMicroseconds(1500);
    //time0 = poll_timer>>2 + (3*time0)>>2;
    time1 = poll_timer;
    //delayMicroseconds(500);
  }
#ifdef DEBUG
  if(debug_timer >= 100)
  {
    //if(!SERIAL_PORT)
    //{
    //  SERIAL_PORT.begin(115200);
    //}
    debug_timer = 0;
    Profile_DebugPrint();
    SERIAL_PORT.printf("GoalUS: %5u\n\r", POLLING_US);
    SERIAL_PORT.printf("PollUS: %5u\n\r", time0);
    SERIAL_PORT.printf("ProcessUS: %5u\n\r", time1);

  }
#endif // DEBUG
}
