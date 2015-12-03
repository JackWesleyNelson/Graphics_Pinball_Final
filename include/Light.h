#pragma once
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
#include <GL/glui.h>			// include our GLUI header
#endif

#ifndef _LIGHT_H_
#define _LIGHT_H_

class Light {
public:
	static int lightID;
	Light();
	Light(float position[4], float ambient[4], float diffuse[4], float specular[4], bool enabled);
    void init();
	void setPosition(float position[4]);
	void setAmbient(float ambient[4]);
	void setDiffuse(float diffuse[4]);
	void setSpecular(float specular[4]);
	void setEnabled(bool enabled);
	void updateLight();

	void spotlight();

private:
	float position[4], ambient[4], diffuse[4], specular[4];
	bool enabled;
	int lightEnum;
};

#endif