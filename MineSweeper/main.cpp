#include "GameBoard.h"

GameBoard board;

// Scene selection variables
bool menuScene = false;
bool gameScene = false;

// Game status variables
int num_of_defusers = 0;
bool gameOver = false;
bool gameWon = false;

// Game Timer variables
bool timer_active = false;
int minutes = 0;
int max_seconds = 0;
int milliseconds = 0;

// Initialization routine.
void setup(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

// OpenGL update function, called every x milliseconds
void update(int value)
{
	glutPostRedisplay();
	
	//update : built-in timer. it is called every 25 milliseconds, so we use the function invocation to increase the milliseconds counter
	// and manage minutes/seconds accordingly
	if(timer_active && !gameOver && !gameWon)
	{
		if (minutes == 0 && max_seconds == 0)
		{
			gameOver = true;
			gameWon = false;
		}
		else
		{
			milliseconds = milliseconds + 25;
			if (milliseconds == 1000)
			{
				milliseconds = 0;
				if(max_seconds == 0)
				{
					max_seconds = 59;
					minutes = minutes - 1;
				}
				else
				{
					max_seconds = max_seconds - 1;
				}
			}
		}
	}
	// Set when to recall the update function again.
	glutTimerFunc(25, update, 0);
}

// Function that draws a simple OpenGL Polygon with a defined width or height multipliers
// used when drawing timer or number of defusers left on screen
void drawPolygon(float width_mult, float height_mult)
{
	float width_dim = 10.0 * width_mult;
	float height_dim = 10.0 * height_mult;
	glBegin(GL_POLYGON);
		glVertex3f(1.0, 1.0, 0.0);
		glVertex3f(width_dim, 1.0, 0.0);
		glVertex3f(width_dim, height_dim, 0.0);
		glVertex3f(1.0, height_dim, 0.0);
	glEnd();
}

// Function to draw a text on the OpenGL window.
void drawText(float translate_x, float translate_y, float scale, float r, float g, float b, string text)
{
	glPushMatrix();
		glColor3f(r, g, b);
		glTranslatef(translate_x, translate_y, 1.0);
		glScalef(scale, scale, 1.0);
		writeStrokeString(GLUT_STROKE_ROMAN, (char*)text.c_str());
	glPopMatrix();
}

// This function allows to draw a visual "button" on Menu screen. With a text label, background color, etc.
void drawButton(float translate_x_t, float translate_y_t, float scale, float translate_x_b, float translate_y_b, string text, float r_b, float g_b, float b_b, float max_x, float max_y)
{
	drawText(translate_x_t, translate_y_t, scale, 0.0, 0.0, 0.0, text);
	//outline
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(translate_x_b, translate_y_b, 1.0);
		glBegin(GL_LINE_LOOP);
			//glLineWidth(3.0);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(max_x, 0.0, 0.0);
			glVertex3f(max_x, max_y, 0.0);
			glVertex3f(0.0, max_y, 0.0);
		glEnd();
	glPopMatrix();
	//background
	glPushMatrix();
		glColor3f(r_b, g_b, b_b);
		glTranslatef(translate_x_b, translate_y_b, 0.5);
		glBegin(GL_POLYGON);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(max_x, 0.0, 0.0);
			glVertex3f(max_x, max_y, 0.0);
			glVertex3f(0.0, max_y, 0.0);
		glEnd();
	glPopMatrix();
}

// Function to draw the timer on screen
void drawTimeCount(float translate_x, float translate_y)
{
	// Timer ///
	drawText(translate_x, translate_y, 0.15, 1.0, 0.0, 0.0, "TIME");
	float trans_x_min = 0.0;
	float trans_x_sec = 0.0;
	if (minutes < 10)
	{
		trans_x_min = translate_x + 50.0;
	}
	else
	{
		trans_x_min = translate_x + 30.0;
	}
	if (max_seconds < 10)
	{
		trans_x_sec = translate_x + 100.0;
	}
	else
	{
		trans_x_sec = translate_x + 80.0;
	}
	drawText(trans_x_min, translate_y - 55.0, 0.25, 1.0, 128.0/255.0, 0.0, to_string(minutes));
	drawText(translate_x + 70.0, translate_y - 55.0, 0.25, 1.0, 128.0 / 255.0, 0.0, ":");
	drawText(trans_x_sec, translate_y - 55.0, 0.25, 1.0, 128.0 / 255.0, 0.0, to_string(max_seconds));
	glPushMatrix();
		glColor3f(0.3, 0.3, 0.3);
		glTranslatef(translate_x, translate_y - 80.0, 1.0);
		drawPolygon(15.0, 7.0);
	glPopMatrix();
}

// Function to draw the defuser counter on screen
void drawDefuseCount(float translate_x, float translate_y)
{
	// Defuser Count
	drawText(translate_x, translate_y, 0.15, 1.0, 0.0, 0.0, "# TO DEFUSE");
	drawText(translate_x + 80.0, translate_y - 55.0, 0.25, 0.0, 1.0, 0.0, to_string(num_of_defusers));

	glPushMatrix();
		glColor3f(0.3, 0.3, 0.3);
		glTranslatef(translate_x, translate_y - 80.0, 1.0);
		drawPolygon(15.0, 7.0);
	glPopMatrix();
}

// Function that calls the Timer or Defuser Counter draw functions
void drawGameOverlay(void)
{
	float defuser_x = 0.0;
	float defuser_y = 0.0;
	float timer_x = 0.0;
	float timer_y = 0.0;

	if (game_mode[0])
	{
		defuser_x = 320.0;
		defuser_y = 680.0;

		if (timer_active)
		{
			timer_x = 20.0;
			timer_y = 680.0;
		}
	}
	else if (game_mode[1])
	{
		defuser_x = 630.0;
		defuser_y = 820.0;

		if (timer_active)
		{
			timer_x = 20.0;
			timer_y = 820.0;
		}
	}
	else if (game_mode[2])
	{
		defuser_x = 945.0;
		defuser_y = 820.0;

		if (timer_active)
		{
			timer_x = 20.0;
			timer_y = 820.0;
		}
	}

	drawDefuseCount(defuser_x, defuser_y);
	if (timer_active)
	{
		drawTimeCount(timer_x, timer_y);
	}
}

// On Game Over or Victory, this function draws the corresponding texts on scree, paierd with the info text.
void drawGameEndedStatus(void)
{
	if (gameOver)
	{
		float game_over_x = 0.0;
		float game_over_y = 0.0;
		float info_x = 0.0;
		float info_y = 0.0;

		if (game_mode[0])
		{
			game_over_x = 47.0;
			game_over_y = 535.0;
			info_x = 66.0;
			info_y = 498.0;
		}
		else if (game_mode[1])
		{
			game_over_x = 200.0;
			game_over_y = 685.0;
			info_x = 219.0;
			info_y = 648.0;
		}
		else if (game_mode[2])
		{
			game_over_x = 350.0;
			game_over_y = 685.0;
			info_x = 369.0;
			info_y = 648.0;
		}
		drawText(game_over_x, game_over_y, 0.3, 1.0, 0.0, 0.0, GAME_OVER_TEXT);
		drawText(info_x, info_y, 0.13, 0.0, 0.0, 0.0, INFO_TEXT);

	}

	if (gameWon)
	{
		float game_won_x = 0.0;
		float game_won_y = 0.0;
		float info_x = 0.0;
		float info_y = 0.0;

		if (game_mode[0])
		{
			game_won_x = 40.0;
			game_won_y = 520.0;
			info_x = 61.0;
			info_y = 498.0;
		}
		else if (game_mode[1])
		{
			game_won_x = 195.0;
			game_won_y = 670.0;
			info_x = 219.0;
			info_y = 648.0;
		}
		else if (game_mode[2])
		{
			game_won_x = 345.0;
			game_won_y = 670.0;
			info_x = 369.0;
			info_y = 648.0;
		}
		drawText(game_won_x, game_won_y, 0.3, 0.0, 204.0 / 255.0, 0.0, GAME_WON_TEXT);
		drawText(info_x, info_y, 0.13, 0.0, 0.0, 0.0, INFO_TEXT);

	}
}

// Menu Scen draw function
void drawMenuScene(void)
{
	// GAME NAME TEXT ON WINDOW
	drawText(15.0, 220.0, 0.23, 0.0, 0.0, 0.0, "MINE SWEEPER!");

	// EASY MODE "BUTTON"
	drawButton(95.0, 155.0, 0.2, 90.0, 150.0, "EASY", 229.0 / 255.0, 1.0, 204.0 / 255.0, 75.0, 30.0);

	// MEDIUM MODE "BUTTON"
	drawButton(85.0, 95.0, 0.2, 80.0, 90.0, "MEDIUM", 1.0, 229.0 / 255.0, 204.0 / 255.0, 105.0, 30.0);

	// HARD MODE "BUTTON"
	drawButton(95.0, 30.0, 0.2, 90.0, 25.0, "HARD", 1.0, 153.0 / 255.0, 153.0 / 255.0, 75.0, 30.0);

	//TIMER TOGGLE
	if (timer_active)
	{
		drawButton(10.0, 10.0, 0.12, 5.0, 5.0, "TIMER", 229.0 / 255.0, 1.0, 204.0 / 255.0, 55.0, 25.0);
	}
	else
	{
		drawButton(10.0, 10.0, 0.12, 5.0, 5.0, "TIMER", 1.0, 153.0 / 255.0, 153.0 / 255.0, 55.0, 25.0);
	}
}

// General OpenGL draw Scene function
// Based on the active scene variable draws the corresponding draw functions
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (menuScene)
	{
		drawMenuScene();
	}
	else if (gameScene)
	{
		drawGameOverlay();

		drawGameEndedStatus();

		// Draw the Game Board
		board.drawGameBoard();
	}

	glFlush();
	glutSwapBuffers();
}

// Function to load a new game
void loadGame(void)
{
	//reset variables
	menuScene = false;
	gameScene = true;
	gameOver = false;
	gameWon = false;
	milliseconds = 0;
	board = GameBoard();
	num_of_defusers = board.getNumOfBombs();

	setup();

	// Set minutes and reshape window based on difficulty setting
	if (game_mode[0])
	{
		minutes = MINUTES_E;
		max_seconds = 0;
		glutReshapeWindow(E_WIDTH, E_HEIGHT);
		glutSetWindowTitle("MINE SWEEPER - EASY");
	}
	else if(game_mode[1])
	{
		minutes = MINUTES_M;
		max_seconds = 0;
		glutReshapeWindow(M_WIDTH, M_HEIGHT);
		glutSetWindowTitle("MINE SWEEPER - MEDIUM");
	}
	else if (game_mode[2])
	{
		minutes = MINUTES_H;
		max_seconds = 0;
		glutReshapeWindow(H_WIDTH, H_HEIGHT);
		glutSetWindowTitle("MINE SWEEPER - HARD");
	}
}

// Function to load the menu Scene
void loadMenu(void)
{
	menuScene = true;
	gameScene = false;
	gameOver = false;
	gameWon = false;
	glutSetWindowTitle("MINE SWEEPER");
	setup();
	glutReshapeWindow(MENU_WIDTH, MENU_HEIGHT);
}

// The resize function, as written here sets the camera to an orthographic view
// And makes it not possible to arbitrarily resize the window to a different aspect.
// Sets the width and height accordingly to the Scene or the difficulty setting
void resize(int w, int h)
{
	float temp_width = 0.0;
	float temp_height = 0.0;

	if (menuScene)
	{
		temp_width = MENU_WIDTH;
		temp_height = MENU_HEIGHT;
	}
	else if (gameScene)
	{

		if (game_mode[0])
		{
			temp_width = E_WIDTH;
			temp_height = E_HEIGHT;
		}
		else if (game_mode[1])
		{
			temp_width = M_WIDTH;
			temp_height = M_HEIGHT;
		}
		else if (game_mode[2])
		{
			temp_width = H_WIDTH;
			temp_height = H_HEIGHT;
		}		
	}

	glViewport(0, 0, temp_width, temp_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, temp_width, 0.0, temp_height, -2.0, 2.0);
	glutReshapeWindow(temp_width, temp_height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

// OpenGL function to mangae the press of keyboard keys
void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: //ESC
		exit(0);
		break;
	case '\t':
		if (gameScene && (gameOver || gameWon))
		{
			loadMenu();
		}
		//cout << "TAB" << endl;
		break;
	case 'n':
		if (gameScene)
		{
			if (gameOver || gameWon)
			{
				loadGame();
			}
		}
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

// OpenGL function to manage the mouse clicks
void mouseControl(int button, int state, int x, int y)
{
	if (menuScene)
	{
		// Only maage left click events
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			//cout << x << "  " << y << endl;
			if (x >= 88 && x <= 162 && y >= 76 && y <= 101)
			{
				// Easy button pressed
				game_mode[0] = true;
				game_mode[1] = false;
				game_mode[2] = false;
				board = GameBoard();
				num_of_defusers = board.getNumOfBombs();
				loadGame();
			}
			else if (x >= 81 && x <= 182 && y >= 138 && y <= 163)
			{
				// Medium button pressed
				game_mode[0] = false;
				game_mode[1] = true;
				game_mode[2] = false;
				board = GameBoard();
				num_of_defusers = board.getNumOfBombs();
				loadGame();
			}
			else if (x >= 91 && x <= 162 && y >= 203 && y <= 227)
			{
				// Hard button pressed
				game_mode[0] = false;
				game_mode[1] = false;
				game_mode[2] = true;
				board = GameBoard();
				num_of_defusers = board.getNumOfBombs();
				loadGame();
			}
			else if (x >= 4 && x <= 59 && y >= 226 && y <= 249)
			{
				//TIMER button toggle
				timer_active = !timer_active;
			}
			
			//cout << game_mode[0] << " " << game_mode[1] << "  " << game_mode[2] << endl;
		}
	}
	else if (gameScene)
	{
		// Only process clicks if not in a Game Over or Victory state
		if (!gameOver && !gameWon)
		{
			int new_y = y;

			// Mouse (0,0) of axis is at top left corner
			// OpenGL draw object space has the (0,0) of axis in bottom left corner
			// So change the y value to use for methods accordingly to the game difficulty height value
			if (game_mode[0])
			{
				new_y = abs(E_HEIGHT - y);
			}
			else if (game_mode[1])
			{
				new_y = abs(M_HEIGHT - y);
			}
			else if (game_mode[2])
			{
				new_y = abs(H_HEIGHT - y);
			}
			// Left click -> select Tile
			if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			{
				gameOver = board.selecTile(x, new_y);
			}
			// Right click -> defuse Tile
			else if ((button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN))
			{
				if (num_of_defusers > 0)
				{
					switch (board.defuseTile(x, new_y))
					{
						// Place defuser
					case 1:
						num_of_defusers--;
						if (num_of_defusers == 0)
						{
							gameOver = !board.checkGameBoard();
							gameWon = !gameOver;
						}
						break;
					// Remove defuser
					case 2:
						num_of_defusers++;
					default:
						break;
					}
				}
			}
		}
	}

	glutPostRedisplay();
}

// Main function
int main(int argc, char** argv){
	srand(time(0));
	menuScene = true;
	gameScene = false;
	gameOver = false;
	gameWon = false;
	
	// timer variables
	timer_active = false;
	milliseconds = 0;
	minutes = 0;
	max_seconds = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowPosition(225, 50);
	glutInitWindowSize(MENU_WIDTH, MENU_HEIGHT);
	glutCreateWindow("MINESWEEPER");
	setup();
	glutKeyboardFunc(processNormalKeys);
	glutIgnoreKeyRepeat(true);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);

	glutMouseFunc(mouseControl);

	glewExperimental = GL_TRUE;
	glewInit();

	glutTimerFunc(25, update, 0);
	glutMainLoop();
	return 0;
}