#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef DEFINES_H
#define DEFINES_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include <algorithm> 
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <ctime>
#include <utility>
#include <fstream>
#include <windows.h>
#include <cctype>
using namespace std;

// Constant values to set the dimension of the gameboard
// and the number of bombs to defuse, depending on the difficulty selected
#define PERC_OF_BOMBS_EASY 10
#define NUM_ROWS_EASY 15
#define NUM_COLS_EASY 15

#define PERC_OF_BOMBS_MEDIUM 17
#define NUM_ROWS_MEDIUM 20
#define NUM_COLS_MEDIUM 25

#define PERC_OF_BOMBS_HARD 26
#define NUM_ROWS_HARD 20
#define NUM_COLS_HARD 35

// Constants for the GameTiles dimensions
#define TILE_WIDTH 30.0
#define TILE_HEIGHT 30.0
#define TILE_STRIDE 1.0

// Constants for the starting positions
// of the game grids depending on the difficulty setting
#define START_X_E 10.0
#define START_Y_E 450.0
#define START_X_M 10.0
#define START_Y_M 605.0
#define START_X_H 10.0
#define START_Y_H 605.0

// Window dimension constants
#define E_WIDTH 490
#define E_HEIGHT 720
#define M_WIDTH 800
#define M_HEIGHT 860
#define H_WIDTH 1115
#define H_HEIGHT 860
#define MENU_WIDTH 256
#define MENU_HEIGHT 256

// Number of minutes for the timer
// Depending on the difficulty setting
#define MINUTES_E 5
#define MINUTES_M 10
#define MINUTES_H 15

// String constants to show on screen
#define GAME_OVER_TEXT "GAME OVER, BUDDY!"
#define GAME_WON_TEXT "AWESOME, YOU WON!"
#define INFO_TEXT "Press N to start again. Press ESC to exit."

#define PI 3.14159265

// Array for the game difficulty setting
// Only one is true at a time
extern bool game_mode[3];

void writeStrokeString(void* font, char* string);
#endif //
