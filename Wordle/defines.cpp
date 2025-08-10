#include "defines.h"

vector<string> word_list = {};

// Function to write a string of characters on screen that could be scaled and rotated.
// For each character in the string, the OpenGL function "glutStrokeCharacter" is called
void writeStrokeString(void* font, char* string)
{
	char* c;
	for (c = string; *c != '\0'; c++)
		glutStrokeCharacter(font, *c);
}