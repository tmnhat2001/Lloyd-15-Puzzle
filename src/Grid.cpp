#include "Grid.hpp"

extern Grid puzzle;

Grid::Grid() {
	selected_index = -1;
	move_direction = -1;
	free_square_position = -1;
	initial_free_square_position = -1;

	for(int i = 0; i < NUM_SQUARES; i++) {
		squares[i] = {i+1, i};
	}

	shuffle();

	n_inversion = calculate_inversion();
}

void Grid::draw() {
	for(int i = 0; i < 15; i++) {
		squares[i].draw();
	}
}

void Grid::shuffle() {
	/*!
	 	 Shuffle the positions of the squares in the grid
	 */

	srand ( time(NULL) ); //initialize the random seed

	// DO NOT use the last element, it's only there to extend the length of the array to be used in random_shuffle
	GLint positions[17] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	std::random_shuffle(&positions[0], &positions[16]);
	int count = 0;
	while (count < NUM_SQUARES) {
		squares[count].set_position(positions[count]);
		count++;
	}

	// The second last element does not get assigned to any square, hence, it's free
	free_square_position = positions[NUM_SQUARES];
	initial_free_square_position = positions[NUM_SQUARES];

	// Reset these variables in case the puzzle is shuffled after the object was created
	selected_index = -1;
	move_direction = -1;

	n_inversion = calculate_inversion();
}

GLint Grid::find_square(GLfloat x, GLfloat y) {
	/*!
		 Check if the given coordinates are within any square.
		 If yes, return the index of the square. Otherwise, return -1.
	 */

	for(int i = 0; i < NUM_SQUARES; i++) {
		if (squares[i].is_in_bounds(x, y)) {
			return i;
		}
	}

	return -1;
}

GLint Grid::is_square_movable(GLint index) {
	/*!
	 	 Check if the given square is movable.
	 	 Return 1 if it can be moved upwards, 2 if it can be moved downwards,
	 	 3 if leftwards, 4 for rightwards and -1 if it cannot be moved.
	 */

	GLint square_pos = squares[index].position;

	// Check if the square can be moved upwards
	if ((square_pos - 4) == free_square_position) {
		return 1;
	}

	// Check if it can be moved downwards
	if ((square_pos + 4) == free_square_position) {
		return 2;
	}

	// Check if it can be moved leftwards
	if ((square_pos - 1) == free_square_position) {
		return 3;
	}

	// Check if it can be move rightwards
	if ((square_pos + 1) == free_square_position) {
		return 4;
	}

	return -1;
}

void slide_motion(int distance) {
	/*!
	 	 Animate the sliding motion of the chosen square
	 */

	if (distance < 5) {
		switch (puzzle.move_direction) {
			case 1:
				puzzle.squares[puzzle.selected_index].translate(0, 0.1, 0);
				break;
			case 2:
				puzzle.squares[puzzle.selected_index].translate(0, -0.1, 0);
				break;
			case 3:
				puzzle.squares[puzzle.selected_index].translate(-0.1, 0, 0);
				break;
			case 4:
				puzzle.squares[puzzle.selected_index].translate(0.1, 0, 0);
				break;
			default:
				break;
		}

		glutPostRedisplay();
		glutTimerFunc(40, slide_motion, distance + 1);
	}
}

void Grid::move_square(GLint index, GLint direction) {
	/*!
	 	 Move a square to a new position
	 */

	if (direction != -1) {
		selected_index = index;
		move_direction = direction;
		slide_motion(0);

		GLint tmp = squares[index].position;
		squares[index].set_position(free_square_position);
		free_square_position = tmp;
		switch (direction) {
			case 1:
				squares[index].translate(0, -0.5, 0);
				break;
			case 2:
				squares[index].translate(0, 0.5, 0);
				break;
			case 3:
				squares[index].translate(0.5, 0, 0);
				break;
			case 4:
				squares[index].translate(-0.5, 0, 0);
				break;
		}
	}
}

int Grid::calculate_inversion() {
	/*
	 * Calculate the number of inversions in the puzzle
	 */

	int total = 0;

	for(int i = 0; i < NUM_SQUARES; i++) {
			if ((squares[i].value - 1) != squares[i].position) {
				// For any misplaced square, its inversion starts at its value - 1.
				// Decrement the inversion count of this square for any preceding square
				// that has smaller value.
				int subtotal = squares[i].value - 1;
				for(int j = 0; j < NUM_SQUARES; j++) {
					if (j != i) {
						if ((squares[j].position < squares[i].position) && (squares[j].value < squares[i].value)) {
							subtotal--;
						}
					}
				}
				total = total + subtotal;
			}
	}

	return total;
}

bool Grid::has_solution() {
	/*
	 * Check if the current puzzle has a solution
	 */

	if ( ( ((initial_free_square_position / 4) % 2) == 0) && ((n_inversion % 2) == 1)) {
		return true;
	} else if ( ( ((initial_free_square_position / 4) % 2) == 1) && ((n_inversion % 2) == 0)) {
		return true;
	} else {
		return false;
	}
}
