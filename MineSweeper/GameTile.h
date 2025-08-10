#pragma once
#ifndef GAME_TILE_H
#define GAME_TILE_H

#include "defines.h"


// Class that defines the Game Tile objects to be shown on screen.
// As private attributes there are the (x, y) coordinates for the first point
// that will define the polygon and (width, height) that represent the tile.
// Then there are some boolean attributes that determine if the tile has been clicked, if has been placed a defuser on
// Or if the GameTile is a Bomb.

class GameTile
{
private:
	int x_coord;
	int y_coord;
	float width;
	float height;
	bool clicked;
	bool isDefused;
	bool isBomb;
public:
	GameTile();
	GameTile(int x, int y);
	~GameTile() {};
	void drawTile(void);
	void drawNumber(int number);
	bool isDefusedT(void);
	void defuse(bool value);
	void select(void);
	bool isClicked(void);
};

#endif