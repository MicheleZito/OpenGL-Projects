#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef DEFINES_H
#define DEFINES_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include <algorithm> 
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <windows.h>
#include <cctype>
#include <unordered_map>
#include <random>

using namespace std;

// Some constant values to be used through the project
// Width, height and stride for the tiles drawed on the game scene
// and to calculate clickable areas.
// Window dimensions to give to openGL functions.
// Strings for Game Over, Victory and info to show on screen.
#define TILE_WIDTH_G 65.0
#define TILE_HEIGHT_G 65.0
#define TILE_STRIDE_G 8.0

#define TILE_WIDTH_K 55.0
#define TILE_HEIGHT_K 70.0
#define TILE_STRIDE_X_K 11.0
#define TILE_STRIDE_Y_K 10.0

#define M_WIDTH 860
#define M_HEIGHT 920

#define GAME_OVER_TEXT "GAME OVER, BUDDY!"
#define GAME_WON_TEXT "AWESOME, YOU WON!"
#define INFO_TEXT "Press N to start again. Press ESC to exit."

// vector for the word list. Instead of reading the file
// and populate this list every time a new game is created
// it is done once in the main function.
extern vector<string> word_list;

// function to write a string of characters on screen that could be scaled and rotated.
void writeStrokeString(void* font, char* string);
#endif
