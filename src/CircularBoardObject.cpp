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