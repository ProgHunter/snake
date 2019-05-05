
#ifndef CONST_H
#include "Arduino.h"
// Constant related to the square dot-matrix size
#define MATRIX_MIN 0
#define MATRIX_MAX 7
#define MATRIX_SIZE 8

// Constants related to the pin mapping of the 74595 chip -> dot-matrix
#define LATCH_PIN 8   //Pin connected to ST_CP(RCLK) of 74HC595
#define CLOCK_PIN 12  //Pin connected to SH_CP(SRCLK) of 74HC595
#define DATA_PIN 11   //Pin connected to DS(SER) of 74HC595

// Constants related to the pin mapping of the joystick
#define JOY_X_PIN A1  // X axis of the joystick
#define JOY_Y_PIN A0  // Y axis of the joystick
#define JOY_B_PIN 7   // Button of the joystick

//dead zone of the joystick
const int16_t DEAD_ZONE = 50;

// Dirrection of a movement
typedef enum mvt_dir { UNKNOWN, LEFT, DOWN, RIGTH, UP } Mvt_dir;

// Game states..
typedef enum game_states { IN_PROGRESS, WIN, LOSE } Game_states;

// Game difficulties
typedef enum game_difficulties { EASE, NORMAL, HARD } Game_difficulties;


#define CONST_H
#endif