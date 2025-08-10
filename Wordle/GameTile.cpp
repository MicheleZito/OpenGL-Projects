#include "GameTile.h"

// Array that defines the rgb triplets to each of the states that the Tile can assume.
static float color_by_state[5][3] = {  {129.0/255.0, 131.0/255.0, 132.0/255.0},
									 {58.0/255.0, 58.0/255.0, 60.0/255.0},
									 {181.0/255.0, 159.0/255.0, 59.0/255.0},
									 {83.0/255, 141.0/255.0, 78.0/255.0},
									 {18.0 / 255.0, 18.0 / 255.0, 19.0 / 255.0}};

// The GameTiles associated with the Trial Words and the "Keyboard" Keys will have different
// width and height, so two constructors are defined. One without parameters, to set default values.
GameTile::GameTile()
{
	this->x_coord = 0;
	this->y_coord = 0;
	this->width = TILE_WIDTH_G;
	this->height = TILE_HEIGHT_G;
	this->tile_state = 0;
}

GameTile::GameTile(int x, int y, float width_, float height_)
{
	this->x_coord = x;
	this->y_coord = y;
	this->width = width_;
	this->height = height_;
	this->tile_state = 0;
}

// Method that when called will draw on the screen the Tile.
// Each tile is composed by a white outline GL_LINE_LOOP
// and the background state-colored GL_POLYGON
// The four points are the same for both of them.
// Since this is a 2D application, with an Orthographic camera, the Z coordinates are set to 1.0
// All the OpenGL structures are defined in an counter-clockwise manner.
void GameTile::drawTile(void)
{
	float x_1 = float(this->x_coord);
	float x_2 = float(this->x_coord) + this->width;
	float y_1 = float(this->y_coord);
	float y_2 = float(this->y_coord) + this->height;
	
	glPushMatrix();
	//White outline
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(x_1, y_1, 1.0);
	glVertex3f(x_2, y_1, 1.0);
	glVertex3f(x_2, y_2, 1.0);
	glVertex3f(x_1, y_2, 1.0);
	glEnd();

	// Background Polygon
	glBegin(GL_POLYGON);
	glColor3f(color_by_state[this->tile_state][0], color_by_state[this->tile_state][1], color_by_state[this->tile_state][2]);
	glVertex3f(x_1, y_1, 1.0);
	glVertex3f(x_2, y_1, 1.0);
	glVertex3f(x_2, y_2, 1.0);
	glVertex3f(x_1, y_2, 1.0);
	glEnd();
	glPopMatrix();
}

// Method that will draw the corresponding Keyboard or Trial word Letter (character)
// above the structure defined in the previous method.
// Here the Z value for the translation is 1.1 to put the printed letter on top of the background.
// The (x,y) coordinates to translate the text get an offset to be placed at the center of the Tile (more or less).
void GameTile::drawLetter(string letter)
{
	if (letter != "")
	{
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		float transl_x = float(this->x_coord) + (this->width / 2.0) - 10.0;
		float transl_y = float(this->y_coord) + (this->height / 2.0) - 5.0;
		
		// ENTER and DEL Keyboard tiles have three characters in their label associated with the button
		// and have to be translated on the x axis in a different way to be more clear to read.
		if (letter.size() > 1)
		{
			transl_x = float(this->x_coord) + 5.0;
		}

		glTranslatef(transl_x, transl_y, 1.1);
		glScalef(0.2, 0.2, 0.2);
		writeStrokeString(GLUT_STROKE_ROMAN, (char*)letter.c_str());
		glPopMatrix();
	}
}

void GameTile::setTileState(int state)
{
	this->tile_state = state;
}