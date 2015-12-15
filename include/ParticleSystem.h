#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_ 1

#include <vector>
#include <math.h>
#include <stdlib.h>

#include "Particle.h"

class ParticleSystem {
public:

	// CONSTRUCTORS / DESTRUCTORS
    ParticleSystem();
	ParticleSystem( float *rules, GLuint s, GLuint *t );
	
	void changeRot( double d, Vector v );
	void addParticle();
	void update();
	void draw();
	void explode( Point p );
	
	private:
		std::vector<Particle> list;
		Point location;
		float maxXorZ;
		float minVelocity, maxVelocity;
		float minLife, maxLife;
		
		
		double rotTheta;
		Vector rotVector;
		GLuint *texs;
		GLuint shader;
};

#endif