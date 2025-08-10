#include "GameBoard.h"

// These vectors are used to store the labels associated with the corresponding Keyboard Key Tiles
vector<string> first_keyboard_row;
vector<string> second_keyboard_row;
vector<string> third_keyboard_row;

GameBoard::GameBoard()
{
    // When a new GameBoard object is created, the list of possible words is shuffled to
    // put more randomness in the current game
    random_shuffle(word_list.begin(), word_list.end());

    // Initialization of the Letter-State dictionary/map. All states are set to 0, meaning that no letter has been used yet.
    // The possible states are [0, 1, 2, 3]: 0 not used, 1 letter not in target word, 2 letter in target but on wrong position, 3 letter match
    this->letters_state = { {'A', 0}, {'B', 0}, {'C', 0}, {'D', 0}, {'E', 0}, {'F', 0}, {'G', 0}, {'H', 0}, {'I', 0}, {'J', 0},
                      {'K', 0}, {'L', 0}, {'M', 0}, {'N', 0}, {'O', 0}, {'P', 0}, {'Q', 0}, {'R', 0}, {'S', 0}, {'T', 0}, {'U', 0},
                      {'V', 0}, {'W', 0}, {'X', 0}, {'Y', 0}, {'Z', 0}};

    first_keyboard_row = { "Q","W","E","R","T","Y","U","I","O","P" };
    second_keyboard_row = { "A","S","D","F","G","H","J","K","L" };
    third_keyboard_row = { "INV","Z","X","C","V","B","N","M","DEL" };

    // Check if the word list has been initialized yet.
    // And select a target word at random
    if (word_list.size() > 0)
    {
        int target_index = rand() % word_list.size();
        this->target_word = word_list.at(target_index);
        //cout << this->target_word << endl;
    }
    else
        this->target_word = "DUMMY";

    // Initialize all private attributes
    this->trial_words = {"","","","","",""};
    this->current_trial = 0;

    this->min_x = {};
    this->max_y = {};
    this->min_y = {};
    this->max_y = {};
    
    this->trial_tiles = {};
    this->trial_words_state = {};
    this->keyboard_tiles = {};

    // Create and Initialize all the Trial words GameTiles.
    // There will be six rows of 5 GameTiles to represent all the trials avaiable.
    // Starting (x, y) coordinates for the first GameTile of the first row.
    float starting_x = 240.0;
    float starting_y = 750.0;

    float previous_y = starting_y;
    for (int i = 0; i < 6; i++)
    {
        vector<GameTile> row_tr = {};

        for (int j = 0; j < 5; j++)
        {
            float current_x = starting_x + float(j) * (TILE_WIDTH_G + TILE_STRIDE_G);
            row_tr.push_back(GameTile(current_x - 0.5, previous_y - 0.5, TILE_WIDTH_G, TILE_HEIGHT_G));
            // Since the Trial words GameTiles are different from the keyboard ones, their background color will be different
            // When not used yet, so the state(s) are set to 4
            row_tr.at(j).setTileState(4);
        }

        // When a row of Trial GameTiles are successfulyy created, calculate the new y-axis offset for the next row
        previous_y = previous_y - (TILE_HEIGHT_G + TILE_STRIDE_G);
        this->trial_tiles.push_back(row_tr);
        this->trial_words_state.push_back({4,4,4,4,4});
    }

    // Create and Initialize all the Keyboard GameTiles.
    // The logic is the same used as previously. with the difference that
    // There are here three rows of GameTiles, and the first has 10 elements while the remaining 2 have 9
    // In addition here, for each row, the corresponding (x, y) coordinates range is computed and stored in the
    // related private vector.
    float start_x_k = 110.0;
    float start_y_k = 250.0;
    float previous_y_k = start_y_k;

    for (int i = 0; i < 3; i++)
    {
        int total_tiles_row = 9;
        if (i == 0)
        {
            total_tiles_row = 10;
        }
        else if(i == 1)
        {
            // X-axis starting offset for the rows with less elements
            start_x_k = 140.0;
        }

        this->min_x.push_back(start_x_k);
        this->min_y.push_back(previous_y_k);
        this->max_y.push_back(previous_y_k + TILE_HEIGHT_K);

        vector<GameTile> row_k = {};
        float current_x = 0.0;

        for (int j = 0; j < total_tiles_row; j++)
        {
            current_x = start_x_k + float(j) * (TILE_WIDTH_K + TILE_STRIDE_X_K);
            row_k.push_back(GameTile(current_x - 0.5, previous_y_k - 0.5, TILE_WIDTH_K, TILE_HEIGHT_K));
        }

        this->max_x.push_back(current_x + TILE_WIDTH_K);
        previous_y_k = previous_y_k - (TILE_HEIGHT_K + TILE_STRIDE_Y_K);
        this->keyboard_tiles.push_back(row_k);

        //cout << i << " : " << endl;
        //cout << "X" << " : " << this->min_x.at(i) << "   " << this->max_x.at(i) << endl;
        //cout << "Y" << " : " << this->min_y.at(i) << "   " << this->max_y.at(i) << endl;
    }
}

// Method to check if the current Trial Word is included in the Words List, i.e. is a legit real word.
// The current trial word is compared to each of the List words. Worst case of O(n) comparisons for each check.
bool GameBoard::checkLegitWord(void)
{
    //bool ans = false;
    for (auto x : word_list)
    {
        if (this->trial_words.at(this->current_trial) == x)
            return true;
    }
    return false;
}

// Append a letter to the current Trial Word, only if the max number of trial words hasn't been reached yet
// And if it hasn't reached the max length yet.
void GameBoard::appendToCurrentTrial(string letter)
{
    if (this->current_trial < 6)
    {
        if (this->trial_words.at(this->current_trial).size() < 5)
        {
            this->trial_words.at(this->current_trial) += letter;
        }
    }

}

// Remove a letter from the current Trial Word, only if it isn't an empty string.
void GameBoard::removeLastFromCurrentTrial(void)
{
    if (this->trial_words.at(this->current_trial).size() > 0)
    {
        this->trial_words.at(this->current_trial).pop_back();
        //cout << this->trial_words.at(this->current_trial) << endl;
    }
}

// Method to draw all the GameTiles on the screen. With their state/color and drawing the corresponding letters on top of them.
void GameBoard::drawGameBoard(void)
{
    // Trial words GameTiles
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < this->trial_tiles.at(0).size(); j++)
        {
            // IF the corresponding Trial word is not an empty string
            // Since this method is called every few milliseconds, it may happen that the current trial word
            // is being written, so not all the 5 characters may have been inserted yet, and since the j index
            // cycles independently on the full dimension, the second part of the if() is to prevent an out of memory access.
            if(this->trial_words.at(i).size() > 0 && j < this->trial_words.at(i).size())
            {
                string temp = { this->trial_words.at(i).at(j) };
                this->trial_tiles.at(i).at(j).setTileState(this->trial_words_state.at(i).at(j));
                this->trial_tiles.at(i).at(j).drawLetter(temp);
            }
            this->trial_tiles.at(i).at(j).drawTile();
        }
    }

    // Keyboard GameTiles
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < this->keyboard_tiles.at(i).size(); j++)
        {
            this->keyboard_tiles.at(i).at(j).drawTile();
            int tile_state = 0;
            string letter_to_draw = "";
            
            switch (i)
            {
                case 0:
                    tile_state = this->letters_state[first_keyboard_row.at(j).at(0)];
                    letter_to_draw = first_keyboard_row.at(j);
                    break;
                case 1:
                    tile_state = this->letters_state[second_keyboard_row.at(j).at(0)];
                    letter_to_draw = second_keyboard_row.at(j);
                    break;
                case 2:
                    if(j > 0 && j < 8)
                        tile_state = this->letters_state[third_keyboard_row.at(j).at(0)];
                    letter_to_draw = third_keyboard_row.at(j);
                    break;
                default:
                    break;
            }
            this->keyboard_tiles.at(i).at(j).setTileState(tile_state);
            this->keyboard_tiles.at(i).at(j).drawLetter(letter_to_draw);
            
        }
    }
}

// This method will be called when a mouse click event will occurr.
// The OpenGL mouse method gives the (x, y) coordinates in which the click happened
// So these two will be used to check if a click occurred in the keboard GameTiles space
// and if so, the corresponding GameTile index will be computed.
// Since in the Keyboard GameTiles there is the ENTER (INV) key, when it is pressed
// The last Trial word must be checked to see if it triggered a Game Over or a Victory event, that's why this method returns
// an integer, that is the value returned by the checkGameBoard() method invocation (if thats the case, otherwise a gamestatus of 2 will be returned).
int GameBoard::selecTile(int x, int y)
{
    int ans = 2;

    int row = 0;
    int col = 0;

    // find the clicked tile depending on the window-space range that is bounded to one of the three rows of keyboard letters
    if (x >= this->min_x.at(0) && x <= this->max_x.at(0) && y >= this->min_y.at(0) && y <= this->max_y.at(0))
    {
        col = min(9, int((x - this->min_x.at(0)) / (TILE_WIDTH_K + TILE_STRIDE_X_K)));
        this->appendToCurrentTrial(first_keyboard_row.at(col));
    }
    else if (x >= this->min_x.at(1) && x <= this->max_x.at(1) && y >= this->min_y.at(1) && y <= this->max_y.at(1))
    {
        col = min(8, int((x - this->min_x.at(1)) / (TILE_WIDTH_K + TILE_STRIDE_X_K)));
        this->appendToCurrentTrial(second_keyboard_row.at(col));
    }
    else if (x >= this->min_x.at(2) && x <= this->max_x.at(2) && y >= this->min_y.at(2) && y <= this->max_y.at(2))
    {
        col = min(8, int((x - this->min_x.at(2)) / (TILE_WIDTH_K + TILE_STRIDE_X_K)));
        if (col == 0)
        {
            //ENTER tile
            this->checkCurrentWord();
            ans = this->checkGameBoard();
        }
        else if (col == 8)
        {
            //DEL tile
            this->removeLastFromCurrentTrial();
        }
        else
        {
            this->appendToCurrentTrial(third_keyboard_row.at(col));
        }
    }
    return ans;
}

// Method to check the Status of the current game. This method return an integer that will onlt be in a fixed range: [0, 1, 2]
// 0 -> game Over, no more trials
// 1 -> correct guess! Victory!
// 2 -> Current trial didnt match the target, or there are more trials to do
int GameBoard::checkGameBoard(void)
{
    int ans = 2;

    // If we haven't exceeded the max number of trials
    if (this->current_trial < 6)
    {
        // if the current Trial word is empty or isn't fully formed, return 2
        if (this->trial_words.at(this->current_trial).size() == 0 || this->trial_words.at(this->current_trial).size() < 5)
        {
            ans = 2;
        }
        else
        {
            // All state variables to the current trial word must be all equal to 3 to determine a Victory status
            bool first_ = this->trial_words_state.at(this->current_trial).at(0) == this->trial_words_state.at(this->current_trial).at(1);
            bool second_ = this->trial_words_state.at(this->current_trial).at(1) == this->trial_words_state.at(this->current_trial).at(2);
            bool third_ = this->trial_words_state.at(this->current_trial).at(2) == this->trial_words_state.at(this->current_trial).at(3);
            bool fourth_ = this->trial_words_state.at(this->current_trial).at(3) == this->trial_words_state.at(this->current_trial).at(4);
            bool fifth_ = this->trial_words_state.at(this->current_trial).at(4) == 3;

            // if all checks succeed, return 1
            if (first_ && second_ && third_ && fourth_ && fifth_)
                return 1;

            // After checking that the current trial word isnt a match to the target
            // we increase the trial counter and check for no more trials to be.
            this->current_trial += 1;
            if (this->current_trial == 6)
                return 0;

            ans = 2;
        }
    }
    else
        return 0;

    return ans;
}

// Method that checks the current trial word, and changes its state flag accordingly,
// so that the player may know if the letters he used are included or not in the target word to guess.
// Since the player can use either the Keyboard GameTiles on screen or his/her keyboard to insert the letters
// this method is public, because the operations made by the selectTile() method have to be accessed elsewhere.
void GameBoard::checkCurrentWord(void)
{
    if (this->current_trial < 6)
    {
        if (this->trial_words.at(this->current_trial).size() == 5)
        {
            if (this->checkLegitWord())
            {
                // To check for the status of the current trial word
                // create a letter-counter dictionary from the target word we want to guess.
                // In this way we will keep track of duplicate letters and manage the state
                // of the current Trial word letters accordingly
                unordered_map<char, int> target_map;
                for (int i = 0; i < 5; i++)
                {
                    char temp_targ = this->target_word.at(i);
                    
                    if (target_map.find(temp_targ) == target_map.end())
                    {
                        //Key not found in dictionary, add a new key
                        target_map.insert({ temp_targ, 1 });
                    }
                    else
                    {
                        target_map.at(temp_targ) += 1;
                    }
                }
                
                int status[5] = { 0, 0, 0, 0, 0 };
                
                // We start checking from the back of the string
                // Because sometimes a trial word that has duplicate letters
                // while the target word may have one instance of those duplicate
                // Since only one of these duplicate letters is included
                // Only one will receive a state 2 at most, when starting from the left side
                // Maybe the last duplicate is already in the right position and would receive a status 3
                // but since the first one received a status 2, that would get a wrong status assignment.
                for (int i = 4; i >= 0; i--)
                {
                    char temp_targ = this->target_word.at(i);
                    char temp_curr = this->trial_words.at(this->current_trial).at(i);
                    if (target_map.find(temp_curr) == target_map.end())
                    {
                        // i-th letter of current trial word not in target
                        // its status will be 1
                        status[i] = 1;
                        // Change the Keyboard GameTiles state
                        this->letters_state[temp_curr] = 1;
                    }
                    else
                    {
                        // i-th letter of current trial word in target
                        // if the letter counter in the dictionary is greater than 0
                        // it means that this current found letter is part of the solution
                        if (target_map.at(temp_curr) > 0)
                        {
                            //If the letter indexes are equals. 
                            if (temp_curr == temp_targ)
                            {
                                status[i] = 3;
                                this->letters_state[temp_curr] = 3;
                            }
                            else
                            {
                                // There is another instance of this letter in target but in 
                                // a different index
                                status[i] = 2;
                                
                                // To update the keyboard tiles background color
                                // we check if that key-letter wasn't a duplicate letter
                                // that was already found matching (status 3)
                                // if not its background is set to the status 2 (yellow)
                                if (this->letters_state[temp_curr] != 3)
                                    this->letters_state[temp_curr] = 2;
                            }
                            // decrease letter count from dictionary
                            target_map.at(temp_curr) -= 1;
                        }
                        else
                        {
                            // Even if the letter was in the solution,
                            // the correct number of instances of that letter
                            // are already been managed (0). So this current letter
                            // status is 1
                            status[i] = 1;
                            //if (this->letters_state[temp_curr] != 3 || this->letters_state[temp_curr] != 2)
                            //    this->letters_state[temp_curr] = 1;
                        }                        
                    }
                    // Update the current trial word tiles with the corresponding status/colors
                    this->trial_words_state.at(this->current_trial).at(i) = status[i];
                }
                // delete dictionary
                target_map.clear();
            }
            else
            {
                // Not a legit word, the current trial word is reset to an empty string
                this->trial_words.at(this->current_trial) = "";
            }
        }
    }
}


string GameBoard::getTargetWord(void)
{
    return this->target_word;
}