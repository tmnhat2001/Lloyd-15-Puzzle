#ifndef SRC_SQUARE_HPP_
#define SRC_SQUARE_HPP_

#include <GL/glut.h>
#include "Shape.hpp"

class Square: public Shape {

	public:
		GLint value, position;
		GLfloat bottom_right_x;
		GLfloat bottom_right_y;

		Square();
		Square(GLint, GLint);
		void draw();
		void set_position(GLint pos);
		bool is_in_bounds(GLfloat x, GLfloat y);
};



#endif /* SRC_SQUARE_HPP_ */
