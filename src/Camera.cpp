// OpenGL / GLUT Libraries we need
#ifdef __APPLE__			// if compiling on Mac OS
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUI/glui.h>			// include our GLUI header
#else					// else compiling on Linux OS
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <math.h>

#include "Camera.h"

//initialize the Camera looking at the origin
Camera::Camera()
{
	this->camdirX = 0;
	this->camdirY = 0;
	this->camdirZ = 0;
	this->CameraRadius = 10;
}

//initialize the Camera looking at the given location
Camera::Camera(float X, float Y, float Z)
{
	this->camdirX = X;
	this->camdirY = Y;
	this->camdirZ = Z;
	this->CameraRadius = 10;
}
//initialize the Camera looking at the given Point
Camera::Camera(Point p)
{
	this->camdirX = p.getX();
	this->camdirY = p.getY();
	this->camdirZ = p.getZ();
	this->CameraRadius = 10;
}

//called when hero moves
void Camera::shiftDir( float heroX, float heroY, float heroZ )
{
	camdirX = heroX;
	camdirY = heroY+2;
	camdirZ = heroZ;
}

//sets camera position based on camdir coordinates, phi, theta, and radius
void Camera::recomputeOrientation()
{
	CameraX = CameraRadius*sin(CameraTheta)*sin(CameraPhi)+camdirX;
	CameraY = -CameraRadius*cos(CameraPhi)+camdirY;
	CameraZ = -CameraRadius*cos(CameraTheta)*sin(CameraPhi)+camdirZ;
}

void Camera::lookAt()
{
	gluLookAt(CameraX, CameraY, CameraZ,
	camdirX, camdirY, camdirZ,
	0, 1, 0);
}

void Camera::setTheta(float angle)
{
	CameraTheta += angle;
}

void Camera::setPhi(float angle)
{
	CameraPhi += angle;
}

void Camera::setRadius(float radius)
{
	CameraRadius += radius;
	if(CameraRadius < 0)
		CameraRadius = .0001;
}

float Camera::getX() {
	return CameraX + camdirX;
}

float Camera::getY() {
	return CameraY + camdirY;
}

float Camera::getZ() {
	return CameraZ + camdirZ;
}