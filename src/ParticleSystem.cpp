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

#include<iostream>
using namespace std;

#include "ParticleSystem.h"

float getRand2() {
    return rand() / (float)RAND_MAX;
}

ParticleSystem::ParticleSystem() {
	this->location = Point( 0, 0, 0 );
	this->maxXorZ = 0.0f;
	this->minVelocity = 0.0f;
	this->maxVelocity = 0.0f;
	this->minLife = 0.0f;
	this->maxLife = 0.0f;
	this->shader = 0;
}

ParticleSystem::ParticleSystem( float *rules, GLuint s, GLuint *t ) {
	this->location = Point( rules[0], rules[1], rules[2] );
	this->maxXorZ = sin(rules[3]*90/M_PI);
	this->minVelocity = rules[4];
	this->maxVelocity = rules[5];
	this->minLife = rules[6];
	this->maxLife = rules[7];
	this->shader = s;
	this->texs = t;
}

void ParticleSystem::changeRot( double d, Vector v ) {
	this->rotTheta = d;
	this->rotVector = v;
}

void ParticleSystem::addParticle() {
	float x = getRand2() * maxXorZ;
	float z = getRand2() * maxXorZ;
	if(getRand2() < 0.5)
		x -= x*2;
	if(getRand2() < 0.5)
		z -= z*2;
	float life = getRand2() * (maxLife-minLife) + minLife;
	float velocity = getRand2() * (maxVelocity-minVelocity) + minVelocity;
	this->list.push_back(Particle( this->location, Vector( x, 2.0f, z ), velocity, life ));
}

void ParticleSystem::explode( Point p ) {
	this->list.push_back( Particle( p, Vector( 1.0f, 1.0f, 0.0f ), this->minVelocity - 0.1, 10 ));
	this->list.push_back( Particle( p, Vector( 1.0f, 0.0f, 0.0f ), this->minVelocity - 0.1, 10 ));
	this->list.push_back( Particle( p, Vector( 1.0f, -1.0f, 0.0f ), this->minVelocity - 0.1, 10 ));
	this->list.push_back( Particle( p, Vector( -1.0f, -1.0f, 0.0f ), this->minVelocity - 0.1, 10 ));
	this->list.push_back( Particle( p, Vector( -1.0f, 0.0f, 0.0f ), this->minVelocity - 0.1, 10 ));
	this->list.push_back( Particle( p, Vector( -1.0f, 1.0f, 0.0f ), this->minVelocity - 0.1, 10 ));
	
	this->list.push_back( Particle( p, Vector( 0.0f, 1.0f, 1.0f ), this->minVelocity - 0.1, 10 ));
	this->list.push_back( Particle( p, Vector( 0.0f, 0.0f, 1.0f ), this->minVelocity - 0.1, 10 ));
	this->list.push_back( Particle( p, Vector( 0.0f, -1.0f, 1.0f ), this->minVelocity - 0.1, 10 ));
	this->list.push_back( Particle( p, Vector( 0.0f, -1.0f, -1.0f ), this->minVelocity - 0.1, 10 ));
	this->list.push_back( Particle( p, Vector( 0.0f, 0.0f, -1.0f ), this->minVelocity - 0.1, 10 ));
	this->list.push_back( Particle( p, Vector( 0.0f, 1.0f, -1.0f ), this->minVelocity - 0.1, 10 ));
}

void ParticleSystem::update() {
	glPushMatrix(); {
	for(int i=0; i<list.size(); i++) {
		list[i].update();
		if(list[i].getLife() < 0) {
			Point p = list[i].getLocation();
			float f = list[i].getVelocity();
			list.erase(list.begin() + i);
			if( f >= 0.07 )
				explode( p );
		}
	}
	}; glPopMatrix();
}

void ParticleSystem::draw() {
	GLenum glewResult = glewInit();
	GLuint *temp = this->texs;
	float v = this->maxVelocity - this->minVelocity;
	float minv = this->minVelocity;
	glPushMatrix(); {
	for(int i=0; i<list.size(); i++) {
		if( list[i].getVelocity() < minv ) {
			glBindTexture(GL_TEXTURE_2D, temp[rand() % 4]);
			glUseProgram( this->shader );
		}
		else if( list[i].getVelocity() >= v * 0.8 + minv ) { 
			glBindTexture(GL_TEXTURE_2D, temp[0]);
			glUseProgram( 0 );
		}
		else if( list[i].getVelocity() >= v * 0.5 + minv ) { 
			glBindTexture(GL_TEXTURE_2D, temp[1]);
			glUseProgram( 0 );
		}
		else if( list[i].getVelocity() >= v * 0.3 + minv ) { 
			glBindTexture(GL_TEXTURE_2D, temp[2]);
			glUseProgram( 0 );
		}
		else { 
			glBindTexture(GL_TEXTURE_2D, temp[3]);
			glUseProgram( 0 );
		}
		list[i].draw( rotTheta, rotVector );
	}
	glEnable(GL_TEXTURE_2D);
	}; glPopMatrix();
}