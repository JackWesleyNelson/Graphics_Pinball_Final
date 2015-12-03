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

#include "Light.h"

//initialize static variables.
int Light::lightID = 0x4000;

Light::Light()
{
	if (lightID == 0x4006) {
		lightID += 0x0001;
	}
	//set light enum for current light
	lightEnum = lightID;
	//increment id for next light
	lightID += 0x0001;
	//set the default position
	position[0] = 0;
	position[1] = 0;
	position[2] = 0;
	position[3] = 1;
	//set the default ambient light
	ambient[0] = 0;
	ambient[1] = 0;
	ambient[2] = 0;
	ambient[3] = 1;
	//set the default diffuse light
	diffuse[0] = 1;
	diffuse[1] = 1;
	diffuse[2] = 1;
	diffuse[3] = 1;
	//set the default specular light
	specular[0] = 0;
	specular[1] = 0;
	specular[2] = 0;
	specular[3] = 1;
	//set the default light state
    enabled = true;
}

void Light::init() {
    //if lighting is not enable, enable it.
    if (!glIsEnabled(GL_LIGHTING)) {
        glEnable(GL_LIGHTING);
    }
    
    //enable the light
    if (!glIsEnabled(lightEnum)) {
        glEnable(lightEnum);
    }
    
    //set the default color values
    glLightfv(lightEnum, GL_DIFFUSE, diffuse);
    glLightfv(lightEnum, GL_AMBIENT, ambient);
    glLightfv(lightEnum, GL_SPECULAR, specular);
}

Light::Light(float position[4], float ambient[4], float diffuse[4], float specular[4], bool enabled)
{
	if (lightID == 0x4006) {
		lightID += 0x0001;
	}
	//set light enum for current light
	lightEnum = lightID;
	//increment id for next light
	lightID += 0x0001;

	//set the given default array values
	for (int i = 0; i < 4; i++) {
		//set the default position
		this->position[i] = position[i];
		//set the default ambient light
		this->ambient[i] = ambient[i];
		//set the default diffuse light
		this->diffuse[i] = diffuse[i];
		//set the default specular light
		this->specular[i] = specular[i];
	}
	//set the default light state
	this->enabled = enabled;

	//if lighting is not enable, enable it.
	if (!glIsEnabled(GL_LIGHTING)) {
		glEnable(GL_LIGHTING);
	}

	//enable the light
	if (!glIsEnabled(lightEnum) && enabled) {
		glEnable(lightEnum);
	}

	//set the default color values
	glLightfv(lightEnum, GL_DIFFUSE, diffuse);
	glLightfv(lightEnum, GL_AMBIENT, ambient);
	glLightfv(lightEnum, GL_SPECULAR, specular);
}

void Light::spotlight() {
	float pos[4] = { 0.0, -0.25, 0.0, 1.0 };
	glLightfv(GL_LIGHT7, GL_POSITION, pos);
	float newCol[4] = { 1, 1, 1, 1 };
	glLightfv(GL_LIGHT7, GL_DIFFUSE, newCol);

	GLfloat dir[4] = { 0, 0, 1, 0 };
	glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, dir);
	glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 12);
	glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 100);
}

void Light::setPosition(float position[4])
{
	for (int i = 0; i < 4; i++) {
		//set the position state
		this->position[i] = position[i];
	}
}

void Light::setAmbient(float ambient[4])
{
	for (int i = 0; i < 4; i++) {
		//set the ambient state
		this->ambient[i] = ambient[i];
	}
}

void Light::setDiffuse(float diffuse[4])
{
	for (int i = 0; i < 4; i++) {
		//set the diffuse state
		this->diffuse[i] = diffuse[i];
	}
}

void Light::setSpecular(float specular[4])
{
	for (int i = 0; i < 4; i++) {
		//set the specular state
		this->specular[i] = specular[i];
	}
}

void Light::setEnabled(bool enabled)
{
		//set the enabled state
		this->enabled = enabled;
}


//update the light to the current values.
void Light::updateLight()
{
	//if lighting is not enable, enable it.
	if (!glIsEnabled(GL_LIGHTING)) {
		glEnable(GL_LIGHTING);
	}

	//enable the light
	if (!glIsEnabled(lightEnum) && enabled) {
		glEnable(lightEnum);
	}

	//set the default color values
	glLightfv(lightEnum, GL_DIFFUSE, diffuse);
	glLightfv(lightEnum, GL_AMBIENT, ambient);
	glLightfv(lightEnum, GL_SPECULAR, specular);
}


