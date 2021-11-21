#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

// Board specifications
#define NUM_ROWS 3
#define NUM_COLS 6
#define PWM_DUTY 0.05


// **** Pin Definitions ****
// LED driver
#define LED_PIN 18
// Thumbstick
#define Y_PIN 23
#define X_PIN 22
#define L3_PIN 21
// Switch matrix row pins (top->bottom)
#define R0_PIN 15
#define R1_PIN 14
#define R2_PIN 13
// Switch matrix column pins (left->right)
#define C0_PIN 11
#define C1_PIN 10
#define C2_PIN 9
#define C3_PIN 8
#define C4_PIN 7
#define C5_PIN 12
// Define arrays for other functions to iterate through
const uint8_t row_pins[NUM_ROWS] = {R0_PIN, R1_PIN, R2_PIN};
const uint8_t col_pins[NUM_COLS] = {C0_PIN, C1_PIN, C2_PIN, C3_PIN, C4_PIN, C5_PIN};



#endif