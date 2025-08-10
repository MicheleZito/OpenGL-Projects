#include "GameBoard.h"

GameBoard board;

bool gameOver = false;
bool gameWon = false;

// Variables used to implement a timer for a possible difficulty management
// used in another project
//bool timer_active = false;
//int minutes = 0;
//int max_seconds = 0;
//int milliseconds = 0;

// Initialization routine.
void setup(void)
{
	// default window background color
	glClearColor(18.0/255.0, 18.0/255.0, 19.0/255.0, 1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

// OpenGL update function, called every x milliseconds, could be used to implement some sort of animation
// or maybe a timer.
void update(int value)
{
	glutPostRedisplay();

	// update function : built-in timer. it is called every 25 milliseconds, 
	// so we use the function invocation to increase the milliseconds counter
	// and manage minutes/seconds accordingly
	/*if (timer_active && !gameOver && !gameWon)
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
				if (max_seconds == 0)
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
	}*/
	// Set when to recall the update function again.
	glutTimerFunc(25, update, 0);
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

// When a game has ended, either with a loss or a victory, the Target word to guess is displayed
// This function allows to draw a box around the word in a way similar to the one used for the GameTiles, nothing new.
void drawBoxedText(float translate_x_t, float translate_y_t, float scale, float translate_x_b, float translate_y_b, string text, float r_b, float g_b, float b_b, float max_x, float max_y)
{
	drawText(translate_x_t, translate_y_t, scale, 1.0, 1.0, 1.0, text);
	//outline
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(translate_x_b, translate_y_b, 1.0);
	glBegin(GL_LINE_LOOP);
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

void drawGameEndedStatus(void)
{
	if (gameOver)
	{
		float game_over_x = 220.0;
		float game_over_y = 860.0;

		drawText(game_over_x, game_over_y, 0.3, 1.0, 0.0, 0.0, GAME_OVER_TEXT);
	}

	if (gameWon)
	{
		float game_won_x = 210.0;
		float game_won_y = 860.0;

		drawText(game_won_x, game_won_y, 0.3, 0.0, 204.0 / 255.0, 0.0, GAME_WON_TEXT);
	}

	if (gameOver || gameWon)
	{
		float info_x = 231.0;
		float info_y = 835.00;
		drawText(info_x, info_y, 0.13, 1.0, 1.0, 1.0, INFO_TEXT);
		drawText(50.0, 650.0, 0.13, 1.0, 1.0, 1.0, "The target was:");
		drawBoxedText(73.0, 610.0, 0.2, 70.0 - 20.0, 610.0 - 20.0, board.getTargetWord(), 129.0 / 255.0, 131.0 / 255.0, 132.0 / 255.0, 130.0, 55.0);
	}
}


void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawGameEndedStatus();
	board.drawGameBoard();

	glFlush();
	glutSwapBuffers();
}

void loadGame(void)
{
	//reset variables
	gameOver = false;
	gameWon = false;
	/*max_seconds = 0;
	milliseconds = 0;*/

	// new GameBoard object
	board = GameBoard();

	setup();
	
	glutReshapeWindow(M_WIDTH, M_HEIGHT);
}

// The resize function, as written here sets the camera to an orthographic view
// And makes it not possible to arbitrarily resize the window to a different aspect.
void resize(int w, int h)
{
	float temp_width = M_WIDTH;
	float temp_height = M_HEIGHT;
	
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
	case 13: // ENTER
		if (!(gameOver || gameWon))
		{
			board.checkCurrentWord();
			switch (board.checkGameBoard())
			{

			case 0:
				gameOver = true;
				gameWon = false;
				break;
			case 1:
				gameOver = false;
				gameWon = true;
				break;
			default:
				break;
			}
		}
		break;
	case 8: //DEL
		if (!(gameOver || gameWon))
		{
			board.removeLastFromCurrentTrial();
		}

		break;
	//case '\t': // TAB
	//	break;
	case 'n':
		
		if (gameOver || gameWon)
		{
			loadGame();
		}
		else
		{
			board.appendToCurrentTrial("N");
		}
		break;
	default:
		// Keys related to alphabet letters, the only keys we care about
		if (int(key) >= 97 && int(key) <= 122)
		{
			string temp = { char(key) };
			transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
			board.appendToCurrentTrial(temp);
		}
		break;
	}

	glutPostRedisplay();
}

// OpenGL function to manage the mouse clicks
void mouseControl(int button, int state, int x, int y)
{
	//cout << x << " " << y << endl;
	if (!gameOver && !gameWon)
	{
		// mouse coordinate in window are different from drawing coordinates
		// Origin of axis in mouse space is in upper left corner.
		// Origin of axis in window drawing space is in lower left corner.
		int new_y = abs(M_HEIGHT - y);

		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			switch (board.selecTile(x, new_y))
			{

			case 0:
				gameOver = true;
				gameWon = false;
				break;
			case 1:
				gameOver = false;
				gameWon = true;
				break;
			default:
				break;
			}
		}
	}

	glutPostRedisplay();
}

// Main function.
int main(int argc, char** argv)
{	
	srand(time(0));
	// timer
	//timer_active = false;
	//milliseconds = 0;
	//minutes = 0;
	//max_seconds = 0;

	// Open word list file and populate corresponding vector
	try
	{
		string word;		
		ifstream MyReadFile("words.txt");

		if (MyReadFile.is_open())
		{
			while (getline(MyReadFile, word))
			{
				// make words to upper case
				transform(word.begin(), word.end(), word.begin(), ::toupper);
				word_list.push_back(word);
			}

			MyReadFile.close();
			//cout << "Word List loaded" << endl;
		}
		else
		{
			cout << "couldn't open file" << endl;
			exit(404);
		}
		
	}
	catch (int errorCode)
	{
		cout << "Error on file opening: " << errorCode << endl;
		exit(69);
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowPosition(225, 50);
	glutInitWindowSize(M_WIDTH, M_HEIGHT);
	glutCreateWindow("WORDLE");
	setup();
	glutKeyboardFunc(processNormalKeys);
	glutIgnoreKeyRepeat(true);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);

	glutMouseFunc(mouseControl);

	glewExperimental = GL_TRUE;
	glewInit();

	loadGame();

	glutTimerFunc(25, update, 0);
	glutMainLoop();
	return 0;
}