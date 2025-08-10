#include "defines.h"

// Array that sets the current game mode: Easy, Medium, Hard.
// Only one is set True at a time.
bool game_mode[3] = {true, false, false};

// Function to write a string of characters on screen that could be scaled and rotated.
// For each character in the string, the OpenGL function "glutStrokeCharacter" is called
void writeStrokeString(void* font, char* string)
{
	char* c;
	for (c = string; *c != '\0'; c++)
		glutStrokeCharacter(font, *c);
}