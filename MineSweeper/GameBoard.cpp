#include "GameBoard.h"

GameBoard::GameBoard()
{
    float starting_x = 0.0;
    float starting_y = 0.0;

    // Set and initialize rows and cols number
    // and starting coordinates for the GameTiles accordingly to the game difficulty setting
    if (game_mode[0])
    {
        this->num_cols = NUM_COLS_EASY;
        this->num_rows = NUM_ROWS_EASY;
        this->max_x = E_WIDTH;
        starting_x = START_X_E + 3.0 * TILE_STRIDE;
        starting_y = START_Y_E;
    }
    else if (game_mode[1])
    {
        this->num_cols = NUM_COLS_MEDIUM;
        this->num_rows = NUM_ROWS_MEDIUM;
        this->max_x = M_WIDTH;
        starting_x = START_X_M + 3.0 * TILE_STRIDE;
        starting_y = START_Y_M;
    }
    else if (game_mode[2])
    {
        this->num_cols = NUM_COLS_HARD;
        this->num_rows = NUM_ROWS_HARD;
        this->max_x = H_WIDTH;
        starting_x = START_X_H + 3.0 * TILE_STRIDE;
        starting_y = START_Y_H;
    }

    // Initialize attributes
    this->min_x = starting_x;
    this->max_y = starting_y + TILE_HEIGHT;
    this->min_y = 0;
    ////////////////
    this->game_board = {};
    this->game_board_check = {};
    this->game_tiles = {};
    this->bomb_coords = {};
    this->defused_coords = {};

    // Initialize all the GameTiles objects that represent the curent GameBoard
    for (int i = 0; i < this->num_rows; i++)
    {
        vector<int> row = {};
        vector<bool> row_b = {};
        vector<GameTile> row_t = {};

        float current_y = starting_y - float(i) * (TILE_HEIGHT + TILE_STRIDE);

        for (int j = 0; j < this->num_cols; j++)
        {
            float current_x = starting_x + float(j) * (TILE_WIDTH + TILE_STRIDE);
            // All int elements of game board initialized to 0 (empty)
            row.push_back(0);
            row_b.push_back(false);
            row_t.push_back(GameTile(current_x, current_y));

            if (i == this->num_rows - 1 && j == this->num_cols - 1)
            {
                // If last element, compute minimum value for Y axis and max value for X axis
                // Of clickable area of GameBoard
                this->min_y = current_y;
                this->max_x = current_x + TILE_WIDTH;
            }
        }
        this->game_board.push_back(row);
        this->game_board_check.push_back(row_b);
        this->game_tiles.push_back(row_t);
    }

    // Compute number of bombs to defuse based on current difficulty setting
    int num_of_bombs = 0;
    if (game_mode[0])
    {
        num_of_bombs = int(float((this->num_rows * this->num_cols) / 100.0) * PERC_OF_BOMBS_EASY);
    }
    else if (game_mode[1])
    {
        num_of_bombs = int(float((this->num_rows * this->num_cols) / 100.0) * PERC_OF_BOMBS_MEDIUM);
    }
    else if (game_mode[2])
    {
        num_of_bombs = int(float((this->num_rows * this->num_cols) / 100.0) * PERC_OF_BOMBS_HARD);
    }
    //cout << "NUM OF BOMBS = " << num_of_bombs << endl;

    // Generate random coordinate where to place the bombs
    // and store the coordinates pair
    // and check if not already occupied
    for (int i = 0; i < num_of_bombs; i++)
    {
        int row_t = rand() % this->num_rows;
        int col_t = rand() % this->num_cols;
        while (this->game_board.at(row_t).at(col_t) == 9)
        {
            row_t = rand() % this->num_rows;
            col_t = rand() % this->num_cols;
        }
        this->game_board.at(row_t).at(col_t) = 9;
        this->bomb_coords.push_back(make_pair(row_t, col_t));
    }

    // Fill the rest of the game board int elements.
    // For each element that is not a bomb
    // check the 8-neighborhood and sum the number of nearby bombs to get
    // corresponding value.
    for (int i = 0; i < this->num_rows; i++)
    {
        for (int j = 0; j < this->num_cols; j++)
        {
            if (this->game_board.at(i).at(j) != 9) // Not a Bomb
            {
                int around_count = 0;

                for (int k = 0; k < c_around.size(); k++)
                {
                    int r_t = i + r_around.at(k);
                    int c_t = j + c_around.at(k);
                    if ((r_t >= 0 && r_t < this->num_rows) && (c_t >= 0 && c_t < this->num_cols))
                    {
                        if (this->game_board.at(r_t).at(c_t) == 9)
                        {
                            around_count += 1;
                        }
                    }
                }

                this->game_board.at(i).at(j) = around_count;
            }
        }
    }

    //cout << "X_min " << this->min_x << "  X_max " << this->max_x << endl;
    //cout << "Y_min " << this->min_y << "  Y_max " << this->max_y << endl;
    //this->print_gameboard();
}

// Debug method to print the int values of the gameboard (i.e. cheating)
void GameBoard::print_gameboard(void)
{
    for (int i = 0; i < this->num_rows; i++)
    {
        for (int j = 0; j < this->num_cols; j++)
        {
            cout << this->game_board.at(i).at(j) << " ";
        }
        cout << endl;
    }
    cout << endl << endl;
}

// Method that returns a boolean value triggering or not a game over, if a bomb was clicked.
// Select a game tile based on (row, col) indices.
bool GameBoard::select_element(int row, int col)
{
    bool answer = false;

    if (row < 0 || row > this->num_rows || col < 0 || col > this->num_cols)
    {
        cout << "Error on input coordinates, retry." << endl;
    }
    else
    {
        // If the selected GameTile has an associated int value that is not Empty (0) or Bomb (9)
        if (this->game_board.at(row).at(col) != 0 && this->game_board.at(row).at(col) != 9)
        {
            this->game_tiles.at(row).at(col).select();
        }
        else if (this->game_board.at(row).at(col) == 9) //BOMB
        {
           answer = true;
           // Expose all bombs on losing. Select all of them.
           for (int i = 0; i < this->bomb_coords.size(); i++)
            {
                this->game_tiles.at(this->bomb_coords.at(i).first).at(this->bomb_coords.at(i).second).select();
            }
        }
        else
        {
            // Empty cell (0)
            // In MineSweeper when an epty cell is clicked, all the nearby empty cells are exposed util a non-empty
            // non-bomb element is reached.

            // Create a queue of pair of indices
            queue<pair<int, int>> zero_queue;
            // Store all pair of indices of elements that will then be automatically selected.
            vector<pair<int, int>> elements_to_show;
            zero_queue.push(make_pair(row, col));
            
            while (!zero_queue.empty())
            {
                // Take first element of queue.
                pair<int, int> temp_coords = zero_queue.front();
                zero_queue.pop();
                elements_to_show.push_back(temp_coords);
                
                // Mark the current element as checked, this way the while loop won't go on forever
                // And will stop correctly
                this->game_board_check.at(temp_coords.first).at(temp_coords.second) = true;
                
                // Check the 8-neighborhood elements
                for (int k = 0; k < c_around.size(); k++)
                {
                    int r_t = temp_coords.first + r_around.at(k);
                    int c_t = temp_coords.second + c_around.at(k);

                    // Check if we are in the legit range of indices and not out of it, i.e. elements on borders or corners
                    if ((r_t >= 0 && r_t < this->num_rows) && (c_t >= 0 && c_t < this->num_cols))
                    {
                        // If not already checked
                        if (!this->game_board_check.at(r_t).at(c_t))
                        {
                            // Enqueue only if an empty cell
                            // In this way as long as we are exploring an area filled with empty cells, we will
                            // keep on expanding the area to show.
                            if (this->game_board.at(r_t).at(c_t) == 0)
                            {
                                zero_queue.push(make_pair(r_t, c_t));
                            }
                            else
                            {
                                // Store indices pair to show and set as checked to not include it more than once
                                elements_to_show.push_back(make_pair(r_t, c_t));
                                this->game_board_check.at(r_t).at(c_t) = true;
                            }
                        }
                    }
                }
            }

            for (int z = 0; z < elements_to_show.size(); z++)
            {
                this->game_tiles.at(elements_to_show.at(z).first).at(elements_to_show.at(z).second).select();
                
                // Reset Check for future Empty cell clicking
                this->game_board_check.at(elements_to_show.at(z).first).at(elements_to_show.at(z).second) = false;
            }
            //this->print_gameboard();
        }
    }
    return answer;
}

// Method to draw all the GameTiles on the screen.
void GameBoard::drawGameBoard(void)
{   
    glPushMatrix();
        // Background black Polygon to place behind the GameTiles
        glColor3f(0.0, 0.0, 0.0);

        // Translate it accordingly on the game difficulty
        if (game_mode[0])
        {
            glTranslatef(10.0, 10.0, 0.9);
        }
        else if (game_mode[1])
        {
            glTranslatef(10.0, 10.0, 0.9);
        }
        else if (game_mode[2])
        {
            glTranslatef(10.0, 10.0, 0.9);
        }
        glBegin(GL_POLYGON);
            glVertex3f(1.0, 1.0, 0.0);
            glVertex3f((this->num_cols * (TILE_WIDTH + TILE_STRIDE) + 5.0 * TILE_STRIDE), 1.0, 0.0);
            glVertex3f((this->num_cols * (TILE_WIDTH + TILE_STRIDE) + 5.0 * TILE_STRIDE), (this->num_rows * (TILE_HEIGHT + TILE_STRIDE) + 8.0 * TILE_STRIDE), 0.0);
            glVertex3f(1.0, (this->num_rows * (TILE_HEIGHT + TILE_STRIDE) + 8.0 * TILE_STRIDE), 0.0);
        glEnd();
    glPopMatrix();

    // For each GameTile, call its draw methods.
    glPushMatrix();
    for (int i = 0; i < this->num_rows; i++)
    {
        for (int j = 0; j < this->num_cols; j++)
        {
            this->game_tiles.at(i).at(j).drawTile();
            this->game_tiles.at(i).at(j).drawNumber(this->game_board.at(i).at(j));
        }
    }
    glPopMatrix();
}

// This method will be called when a mouse click event will occurr.
// The OpenGL mouse method gives the (x, y) coordinates in which the click happened
// So these two will be used to check if a click occurred in the GameTiles space
// and if so, the corresponding GameTile indices will be computed.
// The method returns a boolean value because it checks to see if a Game Over or a Victory event is triggered.
bool GameBoard::selecTile(int x, int y)
{
    bool ans = false;
    int row = 0;
    int col = 0;

    if (x >= this->min_x && x <= this->max_x && y >= this->min_y && y <= this->max_y)
    {
        row = min(this->num_rows - 1, int((this->max_y - y) / (TILE_HEIGHT + TILE_STRIDE)));
        col = min(this->num_cols - 1, int((x - this->min_x) / (TILE_WIDTH + TILE_STRIDE)));
        
        if (!this->game_tiles.at(row).at(col).isClicked())
        {
            ans = this->select_element(row, col);
        }
    }
    //else
    //{
    //    cout << " Out of grid range " << endl;
    //}
    return ans;
}


// This method will be called when a mouse click event will occurr, similarly to the previous method.
// It is used to place or remove already placed defusers on top of GameTiles. So it returns an integer value in [0, 1, 2]
// Where 0 -> false; 1->ok, 2->make avaiable again
int GameBoard::defuseTile(int x, int y)
{
    int ans = 0;
    int row = 0;
    int col = 0;

    if (x >= this->min_x && x <= this->max_x && y >= this->min_y && y <= this->max_y)
    {
        row = min(this->num_rows - 1, int((this->max_y - y) / (TILE_HEIGHT + TILE_STRIDE)));
        col = min(this->num_cols - 1, int((x - this->min_x) / (TILE_WIDTH + TILE_STRIDE)));
        
        // If not already defused, place a defuser there.
        if (!this->game_tiles.at(row).at(col).isDefusedT())
        {   
            if (!this->game_tiles.at(row).at(col).isClicked())
            {
                this->defused_coords.push_back(make_pair(row, col));
                this->game_tiles.at(row).at(col).defuse(true);
                ans = 1;
            }
        }
        // Remove placed Defuser
        else
        {
            int i = 0;
            // Find index in coordinate vector that correspond to selected already defused element
            for (i = 0; i < this->defused_coords.size(); i++)
            {
                // Element found
                if (this->defused_coords.at(i).first == row && this->defused_coords.at(i).second)
                {
                    break;
                }
            }
            
            try
            {
                // Remove the element from vector
                this->defused_coords.erase(this->defused_coords.begin() + i);
                this->game_tiles.at(row).at(col).defuse(false);
                ans = 2;
            }
            catch (int errorCode) {
                cout << "Error occurred: " << errorCode;
            }
        }
        //else
        //{
        //    cout << "already defused!" << endl;
        //}
    }
    //else
    //{
    //    cout << " Out of grid range " << endl;
    //}
    return ans;
}

int GameBoard::getNumOfBombs(void)
{
    return this->bomb_coords.size();
}

// Method to check the Status of the current game.
bool GameBoard::checkGameBoard(void)
{
    bool ans = true;
    int i = 0;

    // If not all bombs are covered with defusers, GameOver event
    for (i = 0; i < this->defused_coords.size(); i++)
    {
        if(this->game_board.at(this->defused_coords.at(i).first).at(this->defused_coords.at(i).second) != 9)
        {
            ans = false;
            break;
        }
    }
    // If Game Over, expose all bombs
    if (!ans)
    {
        this->select_element(this->bomb_coords.at(0).first, this->bomb_coords.at(0).second);
    }
    
    return ans;
}