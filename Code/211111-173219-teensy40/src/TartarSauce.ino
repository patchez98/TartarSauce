#include <Arduino.h>
#include "board.h"
#include "Profile.h"

// NOTE: Must ensure there are at least as many images in Images.h as profiles
#define NUM_PROFILES 4

// Used for setting polling rate
elapsedMicros poll_timer = 0;

void setup() 
{
  // Test for PROGMEM stuff
  //Serial.begin(9600);
  //while(!Serial);
  //delay(2000);
  
  // Pin initialization
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  // Turn on LEDs
  analogWrite(LED_PIN, PWM_DUTY * 256);

  // Initialize keymap and LED
  profile_init();
}

void loop() 
{
  if(poll_timer >= POLLING_US)
  {
    poll_timer = 0;
    
    // Check joystick and send values
    profile_send_joystick();
    // Check keyswitches and send values as necessary
    profile_send_keys();
  }
}
