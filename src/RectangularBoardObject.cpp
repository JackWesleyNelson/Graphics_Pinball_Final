#include "RectangularBoardObject.h"
#include <stdlib.h>

RectangularBoardObject::RectangularBoardObject() {
	x = 0;
	y = 0;
	z = 0;
	deltaX = 0;
	deltaZ = 0;
}

RectangularBoardObject::RectangularBoardObject(int tx, int ty, int tz, int tdx, int tdz){
	x = tx;
	y = ty;
	z = tz;
	deltaX = tdx;
	deltaZ = tdz;
}

RectangularBoardObject::RectangularBoardObject(float tx, float ty, float tz, float tdx, float tdz) {
	x = tx;
	y = ty;
	z = tz;
	deltaX = tdx;
	deltaZ = tdz;
}

double RectangularBoardObject::getX() {
	return x;
}

void RectangularBoardObject::setX(double xt) {
	x = xt;
}

double RectangularBoardObject::getY() {
	return y;
}

void RectangularBoardObject::setY(double yt){
	y = yt;
}

double RectangularBoardObject::getZ() {
	return z;
}

void RectangularBoardObject::setZ(double zt) {
	z = zt;
}

double RectangularBoardObject::getDeltaX() {
	return deltaX;
}

void RectangularBoardObject::setDeltaX(double dxt) {
	deltaX = dxt;
}

double RectangularBoardObject::getDeltaZ() {
	return deltaZ;
}

void RectangularBoardObject::setDeltaZ(double dzt) {
	deltaZ = dzt;
}