#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glew.h>
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
    #include <GLUI/glui.h>			// include our GLUI header
#else					// else compiling on Linux OS
	#include <GL/glew.h>
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
    #include <GL/glui.h>
#endif

#include "CircularBoardObject.h"
#include <stdlib.h>

CircularBoardObject::CircularBoardObject() {
	x = 0;
	y = 0;
	z = 0;
	radius = 0;
}

CircularBoardObject::CircularBoardObject(int tx, int ty, int tz, int tr){
	x = tx;
	y = ty;
	z = tz;
	radius = tr;
}

CircularBoardObject::CircularBoardObject(float tx, float ty, float tz, float tr) {
	x = tx;
	y = ty;
	z = tz;
	radius = tr;
}

double CircularBoardObject::getX() {
	return x;
}

void CircularBoardObject::setX(double xt) {
	x = xt;
}

double CircularBoardObject::getY() {
	return y;
}

void CircularBoardObject::setY(double yt){
	y = yt;
}

double CircularBoardObject::getZ() {
	return z;
}

void CircularBoardObject::setZ(double zt) {
	z = zt;
}

double CircularBoardObject::getRadius() {
	return radius;
}

void CircularBoardObject::setRadius(double rt) {
	radius = rt;
}

void CircularBoardObject::draw() {
	glPushMatrix(); {
		glTranslatef( x, 0, z );
		glRotatef( 90, 1, 0, 0 );
		GLUquadric *quad;
		quad = gluNewQuadric();
		gluQuadricDrawStyle( quad, GLU_FILL);
		glutSolidTorus(radius, radius, 100, 100);
		gluDeleteQuadric(quad);
	}; glPopMatrix();
}