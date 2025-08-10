#pragma once
#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "GameTile.h"

// Class that defines the GameBoard. This class manages all the interactions of the game:
// from checking if a Keyboard key Tile is clicked on screen and append the corresponding letter to the current Trial word,
// to check if the inserted Trial word is a legit word, to check the game status (game over / victory), and so on.
// The private attributes are:
// A dictionary/map to store the status of all the letters used so far in the current game. A letter could not be part of the target word,
//		could be placed in a wrong position, or could match it, or could have not been used yet.
// The target Word to guess in the current game.
// The index of the current trial
// A vector that stores all the trail words used so far. And the corresponding vector for the letter status of each trial word.
// Two vector of vectors of GameTiles to represent the Trial word section or the Keyboard section of the screen.
// Some vectors of int to represent the screen range (min, max) of x and y coordinates to separate the Keyboard Keys rows
//		and manage the click on a such GameTile.
//
// All the methods' names are self explanatory.

class GameBoard
{
private:
	unordered_map<char, int> letters_state;
	string target_word;
	int current_trial;
	vector<string> trial_words;
	vector<vector<int>> trial_words_state;
	vector<vector<GameTile>> trial_tiles;
	vector<vector<GameTile>> keyboard_tiles;
	vector<int> min_x;
	vector<int> max_x;
	vector<int> min_y;
	vector<int> max_y;
	bool checkLegitWord(void);
public:
	GameBoard();
	~GameBoard() {};
	void drawGameBoard(void);
	int selecTile(int x, int y);
	void checkCurrentWord(void);
	int checkGameBoard(void);
	void appendToCurrentTrial(string letter);
	void removeLastFromCurrentTrial(void);
	string getTargetWord(void);
};

#endif