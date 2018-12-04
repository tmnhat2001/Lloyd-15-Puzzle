/**
 *	File Matrix.cpp
 *	Description: implementation of Matrix class
 *
 */

#include "Matrix.hpp"

// mat <- identity matrix
Matrix::Matrix() {
	loadIdentity();
}

void Matrix::loadIdentity() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
				mat[i][j] = 1;
			else
				mat[i][j] = 0;
		}
	}
}

// multiplication  mat <- m * mat
void Matrix::matrixPreMultiply(Matrix* m) {
	GLfloat temp[4][4], sum=0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			sum = 0;
			for (int k = 0; k < 4; k++) {
				sum += m->mat[i][k] * mat[k][j];
			}
			temp[i][j] = sum;
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = temp[i][j];
		}
	}
}

// transpose  mat <- mat'
void Matrix::transpose() {
	for (int i = 0; i < 4; i++) {
		for (int j = i; j < 4; j++) {
			GLfloat tmp = mat[j][i];
			mat[j][i] = mat[i][j];
			mat[i][j] = tmp;
		}
	}
}

// normalize mat
void Matrix::normalize() {
	GLfloat norm = sqrt( pow(mat[0][0], 2) + pow(mat[1][0], 2) + pow(mat[2][0], 2) );
	mat[0][0] = mat[0][0] / norm;
	mat[1][0] = mat[1][0] / norm;
	mat[2][0] = mat[2][0] / norm;

	mat[0][2] = mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1];
	mat[1][2] = mat[2][0] * mat[0][1] - mat[0][0] * mat[2][1];
	mat[2][2] = mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];

	norm = sqrt(mat[0][2] * mat[0][2] + mat[1][2] * mat[1][2] + mat[2][2] * mat[2][2]);
	mat[0][2] = mat[0][2] / norm;
	mat[1][2] = mat[1][2] / norm;
	mat[2][2] = mat[2][2] / norm;

	mat[0][1] = mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2];
	mat[1][1] = mat[2][2] * mat[0][0] - mat[2][0] * mat[0][2];
	mat[2][1] = mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2];

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;
}

// v  <- mat * v
void Matrix::multiplyVector(GLfloat *v) {
	GLfloat result[4];

	for (int i = 0; i < 4; i++) {
		GLfloat total = 0;

		for (int j = 0; j < 4; j++) {
			total = total + mat[i][j] * *(v+j);
		}

		result[i] = total;
	}

	*v = result[0];
	*(v+1) = result[1];
	*(v+2) = result[2];
}

Vector Matrix::mutiplyVector(Vector* v) {
	GLfloat result[3];

	for (int i = 0; i < 3; i++) {
		GLfloat total = 0;

		for (int j = 0; j < 3; j++) {
			if (j == 0) { total += mat[i][j] * v->x; }
			else if (j == 1) { total += mat[i][j] * v->y; }
			else {total += mat[i][j] * v->z; }
		}

		result[i] = total;
	}

	return Vector(result[0], result[1], result[2]);
}

// mat <- rotation matrix (rx, ry, rz, angle)
void Matrix::rotateMatrix(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat angle) {
	GLfloat radians_per_degree = 0.0174533;
	GLfloat angle_in_radians = radians_per_degree * angle;

	GLfloat cos_theta = cos(angle_in_radians);
	GLfloat sine_theta = sin(angle_in_radians);
	GLfloat one_minus_cos_theta = 1 - cos_theta;

	GLfloat vector_magnitude = sqrt(pow(rx, 2) + pow(ry, 2) + pow(rz, 2));
	GLfloat ux = rx / vector_magnitude;
	GLfloat uy = ry / vector_magnitude;
	GLfloat uz = rz / vector_magnitude;

	normalize();

	mat[0][0] = cos_theta + pow(ux, 2) * one_minus_cos_theta;
	mat[0][1] = ux * uy * one_minus_cos_theta - uz * sine_theta;
	mat[0][2] = ux * uz * one_minus_cos_theta + uy * sine_theta;
	mat[0][3] = 0;

	mat[1][0] = uy * ux * one_minus_cos_theta + uz * sine_theta;
	mat[1][1] = cos_theta + pow(uy, 2) * one_minus_cos_theta;
	mat[1][2] = uy * uz * one_minus_cos_theta - ux * sine_theta;
	mat[1][3] = 0;

	mat[2][0] = uz * ux * one_minus_cos_theta - uy * sine_theta;
	mat[2][1] = uz * uy * one_minus_cos_theta + ux * sine_theta;
	mat[2][2] = cos_theta + pow(uz, 2) * one_minus_cos_theta;
	mat[2][3] = 0;

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;
}

