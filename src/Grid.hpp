#ifndef GRID_CPP_
#define GRID_CPP_

#define NUM_SQUARES 15

#include "Square.hpp"
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <Windows.h>

class Grid {
	public:
		Square squares[15];
		GLint initial_free_square_position;
		GLint free_square_position;
		GLint selected_index;
		GLint move_direction;
		int n_inversion;

		Grid();
		void draw();
		void shuffle();
		GLint find_square(GLfloat x, GLfloat y);
		GLint is_square_movable(GLint index);
		void move_square(GLint index, GLint direction);
		int calculate_inversion();
		bool has_solution();

};




#endif /* GRID_CPP_ */
