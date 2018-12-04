#include "Square.hpp"
#include <iostream>

Square::Square() {
	value = 0;
	position = 0;
	bottom_right_x = 0;
	bottom_right_y = 0;
}

Square::Square(GLint val, GLint pos) {
	value = val;

	set_position(pos);
}


void Square::draw() {
	glPushMatrix();
	this->ctmMultiply();

	glBindTexture(GL_TEXTURE_2D, value-1);

	glBegin(GL_QUADS);
		glTexCoord2f(1.0, 0.0); glVertex2f(bottom_right_x, bottom_right_y);
		glTexCoord2f(0.0, 0.0); glVertex2f(bottom_right_x - 0.5, bottom_right_y);
		glTexCoord2f(0.0, 1.0); glVertex2f(bottom_right_x - 0.5, bottom_right_y + 0.5);
		glTexCoord2f(1.0, 1.0); glVertex2f(bottom_right_x, bottom_right_y + 0.5);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex2f(bottom_right_x, bottom_right_y);
		glVertex2f(bottom_right_x - 0.5, bottom_right_y);
		glVertex2f(bottom_right_x - 0.5, bottom_right_y + 0.5);
		glVertex2f(bottom_right_x, bottom_right_y + 0.5);
	glEnd();

	glPopMatrix();
}

void Square::set_position(GLint pos) {
	/*!
	 	 Update the position of the square within the grid
	 */

	position = pos;

	// Normalized coordinate is between -1 and 1
	// Therefore, for the bottom right corner of each square,
	// x is in the range [-0.5, 1] and y is in the range [-1, 0.5]
	bottom_right_x = -0.5 + (pos % 4) * 0.5;
	bottom_right_y = 0.5 - (pos / 4) * 0.5;
}

bool Square::is_in_bounds(GLfloat x, GLfloat y) {
	/*!
	 	 Check if the given coordinates are within the bounds of this square
	 */

	bool in_x_bounds = (x >= (bottom_right_x - 0.5)) && (x <= bottom_right_x);
	bool in_y_bounds = (y >= bottom_right_y) && (y <= (bottom_right_y + 0.5));
	return in_x_bounds && in_y_bounds;
}
