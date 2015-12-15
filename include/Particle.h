#ifndef _PARTICLE_H_
#define _PARTICLE_H_ 1

#include "Point.h"
#include "Vector.h"

class Particle {
public:

	// CONSTRUCTORS / DESTRUCTORS
    Particle();
	Particle( Point p, Vector v, float f, float l );
	
	void update();
	void draw( double d, Vector v );
	void draw();
	Point getLocation();
	float getLife();
	float getVelocity();
	
	private:
		Point location;
		Vector direction;
		float velocity, life;
};

#endif
