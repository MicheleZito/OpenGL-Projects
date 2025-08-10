#include "GameTile.h"

// Array that defines the (r, g, b) values for each digit from 0 to 8
// where 0 means an empty GameTile. 9 Corresponds to a bomb, so it will be displayed differently.
static float colors_by_number[9][3] = { {1.0, 1.0, 1.0}, 
									 {51.0/255.0, 153.0/255.0, 1.0},
									 {0.0 ,153.0 / 255.0 ,0.0},
									 {153.0/255.0, 51.0 / 255.0, 1.0},
									 {204.0/255.0, 204.0/255.0, 0.0},
									 {1.0, 128.0/255.0, 0.0},
									 {1.0, 153.0/255.0, 1.0},
									 {153.0/255.0, 0.0, 76.0/255.0},
									 {0.0, 0.0, 204/255.0}};

GameTile::GameTile()
{
	this->x_coord = 0;
	this->y_coord = 0;
	this->width = TILE_WIDTH;
	this->height = TILE_HEIGHT;
	this->clicked = false;
	this->isBomb = false;
	this->isDefused = false;
}

GameTile::GameTile(int x, int y)
{
	this->x_coord = x;
	this->y_coord = y;
	this->width = TILE_WIDTH;
	this->height = TILE_HEIGHT;
	this->clicked = false;
	this->isBomb = false;
	this->isDefused = false;
}

// Method that when called will draw on the screen the Tile.
// The Polygon changes its color depending on wether it is clicked, it is defused or it is an exploded bomb.
// Since this is a 2D application, with an Orthographic camera, the Z coordinates are set to 1.0 or 1.2
// All the OpenGL structures are defined in an counter-clockwise manner.
void GameTile::drawTile(void)
{
	//Draw a black circle on exploded bomb.
	if (this->isBomb)
	{
		float t = 0;
		int numVertices = 10;
		float radius = 7.0;
		float center_x = float(this->x_coord) + (TILE_WIDTH / 2.0);
		float center_y = float(this->y_coord) + (TILE_HEIGHT / 2.0);
		glBegin(GL_POLYGON);
		glColor3f(0.0, 0.0, 0.0);
		for (int i = 0; i < numVertices; ++i)
		{
			glVertex3f(center_x + radius * cos(t), center_y + radius * sin(t), 1.2);
			t += 2 * PI / numVertices;
		}
		glEnd();
	}
	// Defused Tile: draw white triangle
	if (this->isDefused)
	{
		float x_c = float(this->x_coord);
		float y_c = float(this->y_coord);
		glBegin(GL_TRIANGLES);
			glColor3f(1.0, 1.0, 1.0);
			glVertex3f(x_c + (TILE_WIDTH / 6.0), y_c + (TILE_HEIGHT / 6.0), 1.2);
			glVertex3f(x_c + (TILE_WIDTH / 6.0), y_c + TILE_HEIGHT - (TILE_HEIGHT / 6.0), 1.2);
			glVertex3f(x_c + TILE_WIDTH - (TILE_WIDTH / 6.0), y_c + (TILE_HEIGHT / 2.0), 1.2);
		glEnd();
	}

	float x_1 = float(this->x_coord);
	float x_2 = float(this->x_coord) + this->width;
	float y_1 = float(this->y_coord);
	float y_2 = float(this->y_coord) + this->height;

	glBegin(GL_POLYGON);
	if (this->clicked)
	{	
		if(this->isBomb)
			glColor3f(1.0, 0.0, 0.0);
		else
			glColor3f(1.0, 1.0, 1.0);
	}
	else if (!this->clicked && this->isDefused)
	{
		glColor3f(0.0, 102.0/255.0, 202.0/255.0);
	}
	else
	{
		glColor3f(0.7, 0.7, 0.7);
	}
	glVertex3f(x_1, y_1, 1.0);
	glVertex3f(x_2, y_1, 1.0);
	glVertex3f(x_2, y_2, 1.0);
	glVertex3f(x_1, y_2, 1.0);
	glEnd();
}

// Method that will draw the corresponding number
// above the structure defined in the previous method, if the GameTile has been clicked (0 or 9 won't be drawn).
// Here the Z value for the translation is 1.1 to put the printed number on top of the background.
// The (x,y) coordinates to translate the text get an offset to be placed at the center of the Tile (more or less).
void GameTile::drawNumber(int number)
{
	if (this->clicked)
	{
		this->isDefused = false;
		if (number != 0 && number != 9)
		{
			glPushMatrix();
				glColor3f(colors_by_number[number][0], colors_by_number[number][1], colors_by_number[number][2]);
				glTranslatef(float(this->x_coord) + (TILE_WIDTH/2.0) - 5.0 , float(this->y_coord) + (TILE_HEIGHT/2.0) - 3.0, 1.1);
				glScalef(0.13, 0.13, 1.0);
				string num_s = to_string(number);
				writeStrokeString(GLUT_STROKE_ROMAN, (char*)num_s.c_str());
			glPopMatrix();
		}
		// It is a Bomb
		else if(number == 9)
		{
			this->isBomb = true;
			this->isDefused = false;
		}
	}
}

void GameTile::select(void)
{
	if (!this->isDefused)
	{
		this->clicked = true;
		this->isDefused = false;
	}
}

void GameTile::defuse(bool value)
{
	if (!this->clicked)
	{
		this->clicked = false;
		this->isDefused = value;
	}
}

bool GameTile::isDefusedT(void)
{
	return this->isDefused;
}

bool GameTile::isClicked(void)
{
	return this->clicked;
}