#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Grid.hpp"
#include "pixmap/RGBpixmap.h"

GLint winWidth = 600, winHeight = 600;

Grid puzzle;
GLint selected_square, is_movable;
GLint x_begin, y_begin;
bool moved;
bool show_solvable = false;
RGBpixmap pix[17];

void init(void) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Lloyd's 15 Puzzle");
	glClearColor(0.0, 0.0, 0.0, 1.0);  //Set display-window color to black
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);

	for(int i = 0; i < 15; i++) {
		char filename[15];
		sprintf(filename, "numbers%d.bmp", i+1);
		pix[i].readBMPFile(filename);
		pix[i].setTexture(i);
	}

	pix[15].readBMPFile("solution_exists.bmp");
	pix[15].setTexture(15);


	pix[16].readBMPFile("no_solution.bmp");
	pix[16].setTexture(16);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!show_solvable) {
		puzzle.draw();
	} else {
		if (puzzle.has_solution()) {
			glBindTexture(GL_TEXTURE_2D, 15);
			glBegin(GL_QUADS);
				glTexCoord2f(1.0, 0.0); glVertex2f(0.75, -0.25);
				glTexCoord2f(0.0, 0.0); glVertex2f(-0.75, -0.25);
				glTexCoord2f(0.0, 1.0); glVertex2f(-0.75, 0.25);
				glTexCoord2f(1.0, 1.0); glVertex2f(0.75, 0.25);
			glEnd();
		} else {
			glBindTexture(GL_TEXTURE_2D, 16);
			glBegin(GL_QUADS);
				glTexCoord2f(1.0, 0.0); glVertex2f(0.5, -0.25);
				glTexCoord2f(0.0, 0.0); glVertex2f(-0.5, -0.25);
				glTexCoord2f(0.0, 1.0); glVertex2f(-0.5, 0.25);
				glTexCoord2f(1.0, 1.0); glVertex2f(0.5, 0.25);
			glEnd();
		}
	}

	glFlush();
	glutSwapBuffers();
}

void winReshapeFcn(GLint newWidth, GLint newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	winWidth = newWidth;
	winHeight = newHeight;
}

void keyboard_handler(unsigned char Key, int x, int y) {
	/*!
	 	 Allow user to shuffle the puzzle by pressing "r"
	 	 Allow user to check if the puzzle is solvable by holding "s"
	 */

	switch(Key) {
		case 'r':
			puzzle.shuffle();
			break;
		case 's':
			show_solvable = true;
			break;
		default:
			break;
	}

	glutPostRedisplay();
}

void key_up(unsigned char Key, int x, int y) {
	/*!
	 	 When the "s" key is released, stop showing whether a solution exists
	 */

	switch(Key) {
		case 's':
			show_solvable = false;
			break;
		default:
			break;
	}

	glutPostRedisplay();
}

void normalize_coordinates(float x, float y, GLfloat* x_transformed, GLfloat* y_transformed) {
	/*!
	 	 Normalize mouse click coordinates to the range of [-1, 1]
	 */

	(*x_transformed) = x / winWidth * 2.0 - 1.0;
	(*y_transformed) = - (y / winHeight * 2.0 - 1.0);
}

void mouse_click_handler(int button, int action, int x, int y) {
	if (button == GLUT_LEFT_BUTTON ) {
			if (action == GLUT_DOWN) {
				GLfloat x_transformed, y_transformed;
				normalize_coordinates((float) x, (float) y, &x_transformed, &y_transformed);

				selected_square = puzzle.find_square(x_transformed, y_transformed);
//				cout << "Selected square: " << selected_square << "\n";

				is_movable = puzzle.is_square_movable(selected_square);
//				cout << "Is square movable? " << is_movable << "\n";

				if (is_movable != -1) {
					x_begin = x;
					y_begin = y;
					moved = false;
				}
			}
	}
}

void mouse_move_handler(int x, int y) {
	if (is_movable != -1 && !moved) {
		if (is_movable == 1 && (y < y_begin)) {
			puzzle.move_square(selected_square, is_movable);
			moved = true;
		} else if (is_movable == 2 && (y > y_begin)) {
			puzzle.move_square(selected_square, is_movable);
			moved = true;
		} else if (is_movable == 3 && (x < x_begin)) {
			puzzle.move_square(selected_square, is_movable);
			moved = true;
		} else if (is_movable == 4 && (x > x_begin)) {
			puzzle.move_square(selected_square, is_movable);
			moved = true;
		}

		glutPostRedisplay();
	}
}

int main(int argc, char** argv) {
	setvbuf(stdout, NULL, _IONBF, 0);  //used for prompt Eclipse console output
	setvbuf(stderr, NULL, _IONBF, 0);

	glutInit(&argc, argv);
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard_handler);
	glutKeyboardUpFunc(key_up);
	glutMouseFunc(mouse_click_handler);
	glutMotionFunc(mouse_move_handler);
	glutMainLoop();
	return 0;
}
