
#pragma once
#ifndef GAME_TILE_H
#define GAME_TILE_H

#include "defines.h"

// Class that defines the Game Tile objects to be shown on screen.
// As private attributes there are the (x, y) coordinates for the first point
// that will define the polygon and (width, height) that represent the tile.
// The tile state is an attribute that is used to change the background color of the tile accordingly.
// The possible states are: [0, 1, 2, 3, 4]
// 0 -> neutral for keyboard tiles (light gray)
// 1 -> letter not in target word (black)
// 2 -> letter in target but in different position (yellow)
// 3 -> letter in target in that position (green)
// 4 -> empty trial word tiles (background color)

class GameTile
{
private:
	int x_coord;
	int y_coord;
	float width;
	float height;
	int tile_state;
public:
	GameTile();
	GameTile(int x, int y, float width, float height);
	~GameTile() {};
	void drawTile(void);
	void drawLetter(string letter);
	void setTileState(int state);
};

#endif