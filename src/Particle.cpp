#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include<iostream>
using namespace std;

#include "ParticleSystem.h"
#include "Particle.h"

Particle::Particle() {
	this->location = Point( 0, 0, 0 );
	this->direction = Vector( 0, 0 ,0 );
	this->velocity = 0.0f;
	this->life = 0.0f;
}

Particle::Particle( Point p, Vector v, float f, float l ) {
	this->location = p;
	this->direction = v;
	this->velocity = f;
	this->life = l;
}
	
void Particle::update() {
	Point l = this->location;
	Vector v = this->direction;
	float x = l.getX()+v.getX()*this->velocity;
	float y = l.getY()+v.getY()*this->velocity;
	float z = l.getZ()+v.getZ()*this->velocity;
	this->location = Point( x, y, z );
	this->life -= 0.25;
	if( v.getX() )
	this->direction = Vector( v.getX(), v.getY()-.01, v.getZ() );
}

void Particle::draw( double d, Vector v ) {
	glPushMatrix(); {
	Point l = this->location;
    glTranslatef( l.getX(), l.getY(), l.getZ() );
	glRotatef( d, v.getX(), v.getY(), v.getZ() );
	glScalef( .25, .25, .25);
	GLUquadricObj *myQuad = gluNewQuadric();
	glBegin(GL_QUADS); {
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
	} glEnd();
	}; glPopMatrix();
}

void Particle::draw() {
	glPushMatrix(); {
	Point l = this->location;
    glTranslatef( l.getX(), l.getY(), l.getZ() );
	glRotatef( 90, 0, 1, 0);
	glScalef( .25, .25, .25);
	GLUquadricObj *myQuad = gluNewQuadric();
	glBegin(GL_QUADS); {
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
	} glEnd();
	}; glPopMatrix();
}

Point Particle::getLocation() {
	return this->location;
}

float Particle::getLife() {
	return this->life;
}

float Particle::getVelocity() {
	return this->velocity;
}