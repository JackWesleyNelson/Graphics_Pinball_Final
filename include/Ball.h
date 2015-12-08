#ifndef _BALL_H_
#define _BALL_H_ 1

#include "Vector.h"
#include "Point.h"

class Ball {
public:

	// CONSTRUCTORS / DESTRUCTORS
    Ball();
	Ball( Point l, Vector d, double r );

	// MISCELLANEOUS
    double radius;
    Point location;
    Vector direction;
    
    void draw();
    void moveForward();
    void moveBackward();
    
private:
    double _rotation;
    Point _color; // a cheat because I'm lazy
};

#endif
