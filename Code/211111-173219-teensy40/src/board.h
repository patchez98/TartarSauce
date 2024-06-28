#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include "platformIncludes.h"

// Board specifications
#define NUM_ROWS 3
#define NUM_COLS 6
#define PWM_DUTY 0.5
#define POLLING_HZ 200   // Polling rate of keyboard, in Hz
#define POLLING_US 1000000 / POLLING_HZ

// **** Pin Definitions ****
#ifdef BOARD_TEENSY40
// LED driver
#define LED_PIN 18
// Thumbstick
#define Y_PIN 23
#define X_PIN 22
#define L3_PIN 21
// General-purpose switches
#define SW0_PIN 20
#define SW1_PIN 19
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

// Communications ports
#define SERIAL_PORT Serial

#elif (defined BOARD_RP2040)
// NOTE: Labels for each pin are found on the schematic symbol.
// LED driver
#define LED_PIN 5
// Thumbstick
#define Y_PIN A1
#define X_PIN A0
#define L3_PIN 18
#define JOY_DETECT_PIN 17
// General-purpose switches
#define SW0_PIN 28
#define SW1_PIN 29
// Switch matrix row pins (top->bottom)
#define R0_PIN 6
#define R1_PIN 7
#define R2_PIN 14
// Switch matrix column pins (left->right)
#define C0_PIN 13
#define C1_PIN 11
#define C2_PIN 10
#define C3_PIN 9
#define C4_PIN 8
#define C5_PIN 12

// Communications ports
#define SERIAL_PORT Serial1

#endif // Board pin mapping

// Define arrays for other functions to iterate through
const uint8_t row_pins[NUM_ROWS] = {R0_PIN, R1_PIN, R2_PIN};
const uint8_t col_pins[NUM_COLS] = {C0_PIN, C1_PIN, C2_PIN, C3_PIN, C4_PIN, C5_PIN};



#endif