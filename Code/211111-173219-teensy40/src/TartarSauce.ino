#include "board.h"
#include "Profile.h"

// NOTE: Must ensure there are at least as many images as profiles
#define NUM_PROFILES 4

//Only define these once for speed
static uint16_t vert;
static uint16_t hori;

void setup() {
  // Test for PROGMEM stuff
  Serial.begin(9600);
  while(!Serial);
  delay(2000);
  
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(L3_PIN, INPUT_PULLUP);
  for (int i = 0; i < NUM_ROWS; i++)
  {
    pinMode(i, INPUT_PULLDOWN);
  }
  for (int i = 0; i < NUM_COLS; i++)
  { 
    digitalWrite(i, LOW);
    pinMode(i, OUTPUT);
  }
  //
  analogWrite(LED_PIN, PWM_DUTY * 256);
}

void loop() {
  /*
  //Read and update analog values
  //Vertical (Y) axis
  vert = analogRead(Y_PIN);
  Joystick.Y(vert);
  //Horizontal (X) axis
  hori = analogRead(X_PIN);
  Joystick.X(hori);
  //L3
  Joystick.button(1, !digitalRead(L3_PIN));
  
  //Detect keypresses
  for(int i=0; i < NUM_COLS; i++)
  {
    digitalWrite(col_pins[i], HIGH);
    for(int j=0; j < NUM_ROWS; j++)
    {

      digitalRead(row_pins[j]);
    }
  }
  */
  
}
