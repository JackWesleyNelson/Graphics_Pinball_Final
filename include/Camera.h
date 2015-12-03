#pragma once
#ifndef _Camera_H_
#define _Camera_H_

#include "Point.h"

class Camera {
public:
	Camera();
	Camera(float CameraX, float CameraY, float CameraZ);
	Camera(Point p);
	
	void shiftDir(float heroX, float heroY, float heroZ);
	void recomputeOrientation();
	void lookAt();
	void setTheta(float angle);
	void setPhi(float angle);
	void setRadius(float radius);

	float getX();
	float getY();
	float getZ();
	
private:
	float CameraX, CameraY, CameraZ;            // Camera position in cartesian coordinates
	float CameraTheta, CameraPhi;               // Camera DIRECTION in spherical coordinates
	float camdirX, camdirY, camdirZ;                     // Camera DIRECTION in cartesian coordinates
	float CameraRadius;			//distance from camera to focus Point
};
#endif