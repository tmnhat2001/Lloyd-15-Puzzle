#include "Shape.hpp"

Shape::Shape() {
	reset();
}

Shape::~Shape() { };

void Shape::reset() {
    mc.loadIdentity();
	s = 1;
};

Matrix Shape::getMC() {
	return  mc;
};

void Shape::translate(GLfloat tx, GLfloat ty, GLfloat tz) {
	mc.mat[0][3] += tx;
	mc.mat[1][3] += ty;
	mc.mat[2][3] += tz;
	mc.mat[3][3] = 1;
}


void Shape::scaleChange(GLfloat x) {
	s += x;
}

/**
 * Shape::ctm_multiply() multiply this matrix to openGL current matrix
 */
void Shape::ctmMultiply() {
	mc.transpose();
	glMultMatrixf(&mc.mat[0][0]);
	mc.transpose();

}

void Shape::rotate(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat angle) {
	Matrix* rotation_matrix = new Matrix();
	rotation_matrix->rotateMatrix(rx, ry, rz, angle);
	mc.matrixPreMultiply(rotation_matrix);
}

void Shape::rotateMC(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat angle) {
	GLfloat x0 = mc.mat[0][3],y0 = mc.mat[1][3], z0 = mc.mat[2][3];
	rotate(rx, ry, rz, angle);
	mc.mat[0][3] = x0;
	mc.mat[1][3] = y0;
	mc.mat[2][3]  =z0;
}

void Shape::rotateOrigin(GLfloat rx, GLfloat ry, GLfloat rz, GLfloat angle) {
	Matrix m;
	m.rotateMatrix(rx, ry, rz, angle);
	GLfloat v[4];
	v[0] = mc.mat[0][3];
	v[1] = mc.mat[1][3];
	v[2] = mc.mat[2][3];
	v[3] = mc.mat[3][3];
	m.multiplyVector(v);
	mc.mat[0][3] = v[0];
	mc.mat[1][3] = v[1];
	mc.mat[2][3] = v[2];
	mc.mat[3][3] = v[3];
}

