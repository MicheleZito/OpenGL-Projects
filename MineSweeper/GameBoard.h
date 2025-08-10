#pragma once
#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "GameTile.h"

// When checking a position for what number to assign on the board,
// The 8 position around it are checked. These two vectors define the (x,y)
// Coordinates to sum to the current element, to get the corresponding value
// On the border
//	(-1, -1)  (-1, 0)   (-1, 1)
//  ( 0, -1)  ( x, y)   ( 0, 1)
//  ( 1, -1)  ( 1, 0)   ( 1, 1) 
static const vector<int> c_around = { -1, 0, 1, -1, 1, -1, 0, 1 };
static const vector<int> r_around = { -1, -1, -1, 0, 0, 1, 1, 1 };


// Class that defines the GameBoard. This class manages all the interactions of the game:
// from checking if a GameTile is clicked on screen (defusing or not),
// to check the game status (game over / victory), and so on.
// The private attributes are:
// A vector of vectors of int that represent the 2D board game elements.
// A vector of vectors of booleans that represent the 2D board game elements when clicking on a 0-tile (and then select and reveal all linked elements).
// A vector of vectors of GameTiles to represent the game board tiles.
// A vector of pair of indices/coordinates of all bombs on the gameboard.
// A vector of pair of indices/coordinates of all defuser devices placed on the gameboard.
// Integers that define the number of rows and coulmns of the current game board and the clickable gameboard space area on window
//
// All the methods' names are self explanatory.
class GameBoard
{
private:
	vector<vector<int>> game_board;
	vector<vector<bool>> game_board_check;
	vector<vector<GameTile>> game_tiles;
	vector<pair<int, int>> bomb_coords;
	vector<pair<int, int>> defused_coords;
	int num_rows;
	int num_cols;
	int min_x;
	int max_x;
	int min_y;
	int max_y;
	void print_gameboard(void);
	bool select_element(int row, int col);
public:
	GameBoard();
    ~GameBoard(){};
	void drawGameBoard(void);
	bool selecTile(int x, int y);
	int defuseTile(int x, int y);
	int getNumOfBombs(void);
	bool checkGameBoard(void);
};

#endif