// HEADERS /////////////////////////////////////////////////////////////////////

// OpenGL / GLUT Libraries we need
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

#include <SOIL/SOIL.h>

// C Libraries we need
#include <time.h>			// for time() to seed our RNG
#include <stdio.h>			// allow to print to terminal
#include <stdlib.h>			// access to rand() and exit()
#include <math.h>			// for cosf(), sinf(), etc.


// C++ Libraries we'll use
#include <fstream>			// we'll use ifstream
#include <string>			// for, well strings!
#include <vector>			// and vectors (the storage container, not directional)
#include <iostream>
#include <sstream>
#include <string.h>
using namespace std;


// Headers We've Written
#include "Object.h"
#include "Camera.h"

// Additional headers
#include "Point.h"
#include "Vector.h"
#include "Ball.h"
#include "CircularBoardObject.h"
#include "RectangularBoardObject.h"
#include "Shader_Utils.h"
#include "MD5/md5model.h"
#include "MD5/md5mesh.h"
#include "MD5/md5anim.h"
#include "Particle.h"
#include "ParticleSystem.h"
#include "Shader.h"

// For some reason this math header fixes the abs() error
#include <cmath>

// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

static size_t windowWidth = 640;
static size_t windowHeight = 480;
static float aspectRatio;
//animation variables
int animated = 0;

struct md5_model_t md5file;
struct md5_anim_t md5anim;

struct md5_joint_t *skeleton = NULL;
struct anim_info_t animInfo;

bool DISPLAY_WIREFRAME = false;
bool DISPLAY_SKELETON = false;

int winMain;						//Window declaration

GLint leftMouseButton, rightMouseButton;    // status of the mouse buttons
int mouseX = 0, mouseY = 0;                 // last known X and Y of the mouse

GLuint environmentDL;			

GLint menuId;

//Camera variables
Camera camera;
float globalRadius = 300, globalTheta = (M_PI / 2.0f), globalPhi= (M_PI / 1.7f);
bool topView = false;

//Object Variables
Object *table;
float tableX = 50.0f, tableZ = 25.0f;

//Textures
GLuint textures[10];

//Game Variables
bool started, animating, charging, LbumpOn = false, RbumpOn = false;
float charge = 0, score = 0;
Point Lbump = Point( 42.5, 0.0, 10.0 ), Rbump = Point( 42.5, 0.0, -10.0 );

//Particle System Variables
GLuint particleShaderHandle;
float rules[9];
ParticleSystem pSystem;
bool fountainOn = true;

//Ball object (can be replaced by system of balls for multi-ball system)
Ball gameBall;

// Vector of circular objects on table
vector<CircularBoardObject> circular_objects;

// Vector of square objects on table
vector<RectangularBoardObject> rectangular_objects;

// Current setup for testing
bool ballEnabled = true;  // Current setting = false so it shouldn't interfere with anything else


// END OF GLOBAL VARIABLES

// getRand() ///////////////////////////////////////////////////////////////////
//
//  Simple helper function to return a random number between 0.0f and 1.0f.
//
////////////////////////////////////////////////////////////////////////////////
float getRand() {
    return rand() / (float)RAND_MAX;
}

//Initialize a new game ball
void initialize() {
	gameBall = Ball( Point(tableX-20, 0.0, -tableZ+2.0), Vector(0.1, 0.0, 0.0), 2.0 );
	gameBall.direction = Vector( 1, 0, 0 );
	gameBall.velocity = 0.0;
}

//drawGrid()
//////////////////////////////////////////////////////////////
//
//	Draws the initial pinball table setup
//
////////////////////////////////////////////////////////////////////////////////
void drawGrid() {
	glPushMatrix(); {
		glDisable( GL_LIGHTING );
			glBegin(GL_LINES); {
			for (int i = -tableX; i <= tableX; i +=5)
				for (int j = -tableZ; j <= tableZ; j +=5) {
					/*if( j % 5 == 0 && i % 5 == 0)
						glColor3f( 1, 0, 0 );
					else glColor3f( 1, 1, 1 );*/
					glVertex3f(i, 0, -25);
					glVertex3f(i, 0, 25);

					glVertex3f(50, 0, j);
					glVertex3f(-50, 0, j);
				}
			}; glEnd();
		glEnable( GL_LIGHTING );
	} glPopMatrix();
}

//drawBorders()
//////////////////////////////////////////////////////////////
//
//	Draws the initial pinball table setup
//
////////////////////////////////////////////////////////////////////////////////
void drawBorders() {
	glPushMatrix(); {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		GLUquadricObj *myQuad2 = gluNewQuadric();
		glBegin(GL_QUADS); {
			glNormal3f( 1.0f, 0.0f, 0.0f);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(-tableX, 0, -tableZ);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(-tableX, 0, tableZ);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(-tableX, 1, tableZ);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(-tableX, 1, -tableZ);
			
			glNormal3f( 0.0f, 1.0f, 0.0f);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(-tableX-1, 0, -tableZ-1);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(-tableX-1, 0, tableZ+1);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(-tableX, 1, tableZ);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(-tableX, 1, -tableZ);
			
			glNormal3f( 0.0f, 0.0f, -1.0f);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(-tableX, 0, -tableZ);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX, 0, -tableZ);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX, 1, -tableZ);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(-tableX, 1, -tableZ);
			
			glNormal3f( 0.0f, 1.0f, 0.0f);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(-tableX-1, 0, -tableZ-1);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX+1, 0, -tableZ-1);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX, 1, -tableZ);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(-tableX, 1, -tableZ);
			
			glNormal3f( -1.0f, 0.0f, 0.0f);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX, 0, -tableZ);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX, 0, -5);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX, 1, -5);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX, 1, -tableZ);
			
			glNormal3f( 0.0f, 1.0f, 0.0f);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX+1, 0, -tableZ-1);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX+1, 0, -5);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX, 1, -5);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX, 1, -tableZ);
			
			glNormal3f( -1.0f, 0.0f, 0.0f);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX, 0, tableZ);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX, 0, 5);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX, 1, 5);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX, 1, tableZ);
			
			glNormal3f( 0.0f, 1.0f, 0.0f);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX+1, 0, tableZ+1);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX+1, 0, 5);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX, 1, 5);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX, 1, tableZ);
			
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(-tableX, 0, tableZ);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX, 0, tableZ);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX, 1, tableZ);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(-tableX, 1, tableZ);
			
			glNormal3f( 0.0f, 1.0f, 0.0f);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(-tableX-1, 0, tableZ+1);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX+1, 0, tableZ+1);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX, 1, tableZ);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(-tableX, 1, tableZ);
			
			glNormal3f( 0.0f, 0.0f, -1.0f);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX, 0, -tableZ+4);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(-tableX+15, 0, -tableZ+4);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(-tableX+15, 1, -tableZ+4);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX, 1, -tableZ+4);
			
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX, 0, -tableZ+6);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(-tableX+15, 0, -tableZ+6);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(-tableX+15, 1, -tableZ+6);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX, 1, -tableZ+6);
			
			glNormal3f( 0.0f, 1.0f, 0.0f );
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX, 1, -tableZ+6);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(-tableX+15, 1, -tableZ+6);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(-tableX+15, 1, -tableZ+4);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX, 1, -tableZ+4);
			
			glNormal3f( 1.0, 0.0 , 1.0 );
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(-tableX, 0, -tableZ+5);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(-tableX, 1, -tableZ+5);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(-tableX+5, 1, -tableZ);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(-tableX+5, 0, -tableZ);
			
			glNormal3f( 0.0f, 1.0f, 0.0f );
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-tableX, 1, -tableZ+5);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-tableX+5, 1, -tableZ);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-tableX, 1, -tableZ);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-tableX, 1, -tableZ+5);
			
			glNormal3f( -1.0, 0.0 , -1.0 );
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX-5, 0, 5);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX-5, 1, 5);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX-25, 1, tableZ);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX-25, 0, tableZ);
			
			glNormal3f( 0.0, 1.0 , 0.0 );
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX-5, 1, 5);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX-25, 1, tableZ);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX, 1, tableZ);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX, 1, 5);
			
			glNormal3f( -1.0, 0.0 , 1.0 );
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX-5, 0, -5);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX-5, 1, -5);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX-20, 1, -20);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX-20, 0, -20);
			
			glNormal3f( 0.0, 1.0 , 0.0 );
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX-5, 1, -5);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX-20, 1, -20);
			glTexCoord2f(8.0f, 1.0f);
			glVertex3f(tableX, 1, -20);
			glTexCoord2f(8.0f, 0.0f);
			glVertex3f(tableX, 1, -5);
			
		}; glEnd();
		glDisable( GL_TEXTURE_2D );
	}; glPopMatrix();
}

//drawSky()
//////////////////////////////////////////////////////////////
//
//	Draws the skybox
//
////////////////////////////////////////////////////////////////////////////////
void drawSky() {
	glPushMatrix(); {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		
		GLUquadricObj *myQuad2 = gluNewQuadric();
		glBegin(GL_QUADS); {
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.33f, 0.33f);
			glVertex3f(-300.0f, -300.0f, -300.0f);
			glTexCoord2f(0.33f, 0.66f);
			glVertex3f( -300.0f, 300.0f, -300.0f);
			glTexCoord2f(0.0f, 0.66f);
			glVertex3f( -300.0f,  300.0f, 300.0f);
			glTexCoord2f(0.0f, 0.33f);
			glVertex3f(-300.0f,  -300.0f, 300.0f);
			
			glNormal3f( 1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.33f, 0.33f);
			glVertex3f(-300.0f, -300.0f, -300.0f);
			glTexCoord2f(0.33f, 0.66f);
			glVertex3f( -300.0f, 300.0f, -300.0f);
			glTexCoord2f(0.5f, 0.66f);
			glVertex3f( 300.0f, 300.0f, -300.0f);
			glTexCoord2f(0.5f, 0.33f);
			glVertex3f( 300.0f, -300.0f, -300.0f);
			
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.5f, 0.33f);
			glVertex3f( 300.0f, -300.0f, -300.0f);
			glTexCoord2f(0.5f, 0.66f);
			glVertex3f( 300.0f, 300.0f, -300.0f);
			glTexCoord2f(0.75f, 0.66f);
			glVertex3f( 300.0f,  300.0f, 300.0f);
			glTexCoord2f(0.75f, 0.33f);
			glVertex3f( 300.0f,  -300.0f, 300.0f);
			
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(1.0f, 0.33f);
			glVertex3f( -300.0f, -300.0f, 300.0f);
			glTexCoord2f(1.0f, 0.66f);
			glVertex3f( -300.0f, 300.0f, 300.0f);
			glTexCoord2f(0.75f, 0.66f);
			glVertex3f( 300.0f, 300.0f, 300.0f);
			glTexCoord2f(0.75f, 0.33f);
			glVertex3f( 300.0f, -300.0f, 300.0f);
			
			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.5f, 1.0f);
			glVertex3f( -300.0f, 300.0f, -300.0f);
			glTexCoord2f(0.75f, 1.0f);
			glVertex3f( -300.0f, 300.0f, 300.0f);
			glTexCoord2f(0.75f, 0.66f);
			glVertex3f( 300.0f, 300.0f, 300.0f);
			glTexCoord2f(0.5f, 0.66f);
			glVertex3f( 300.0f, 300.0f, -300.0f);
			
			glNormal3f( 0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.5f, 0.0f);
			glVertex3f( -300.0f, -280.0f, -300.0f);
			glTexCoord2f(0.75f, 0.0f);
			glVertex3f( -300.0f, -280.0f, 300.0f);
			glTexCoord2f(0.75f, 0.33f);
			glVertex3f( 300.0f, -280.0f, 300.0f);
			glTexCoord2f(0.5f, 0.33f);
			glVertex3f( 300.0f, -280.0f, -300.0f);
		}; glEnd();
		glDisable( GL_TEXTURE_2D );
	}; glPopMatrix();
}

//drawTitle()
void drawTitle() {
	glDisable(GL_LIGHTING);
	char title[64] = "PINBALL";
	char instructions[64] = "Press Any Key To Start";
	
	glPushMatrix(); {
	
		// where we want it written
		glRotatef(90, 0, 1, 0);
		glRotatef(-15, 1, 0, 0);
		glTranslatef(-25, 0, 250);
	
		// how big we want it
		glPushMatrix(); {
			glScalef(.1, .1, .1);
			for (int c = 0; title[c] != 0; ++c)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, title[c]);
		}; glPopMatrix();
		
		glTranslatef(8, -5, 0);
		glScalef(.02, .02, .02);
			for (int c = 0; instructions[c] != 0; ++c)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, instructions[c]);
	}; glPopMatrix();
	glEnable(GL_LIGHTING);
	
}

void drawScore() {
	glDisable(GL_LIGHTING);
	char title[64] = "SCORE:";
	char s[6];
	sprintf(s, "%f", score);
	
	glPushMatrix(); {
	
		// where we want it written
		glRotatef(90, 0, 1, 0);
		glTranslatef(-30, 10, -tableZ - 35);
	
		// how big we want it
		glPushMatrix(); {
			glScalef(.05, .05, .05);
			for (int c = 0; title[c] != 0; ++c)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, title[c]);
		}; glPopMatrix();
		glTranslatef(25, 0, 0);
		glPushMatrix(); {
			glScalef(.05, .05, .05);
			for(int i = 0; i < 6; i++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, s[i]);
		}; glPopMatrix();
	}; glPopMatrix();
	glEnable(GL_LIGHTING);
}

//drawCharge()
//////////////////////////////////////////////////////////////
//
//	Draws the charge of the ball spring
//
////////////////////////////////////////////////////////////////////////////////
void drawCharge() {
	glPushMatrix(); {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[3]);
		glTranslatef( tableX+2, 0.0f, -tableZ );
		GLUquadricObj *myQuad2 = gluNewQuadric();
		glBegin(GL_QUADS); {
			for(int i=0; i<charge; i++) {
				glNormal3f( 0.0f, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(0, 0, 0+(2*i));
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(1, 0, 0+(2*i));
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(1, 0, 1+(2*i));
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(0, 0, 1+(2*i));
			}
		}; glEnd();
		glDisable(GL_TEXTURE_2D);
	}; glPopMatrix();
}

void drawBumpers() {
	glPushMatrix(); {
		glPushMatrix(); {
			glTranslatef( Lbump.getX(), Lbump.getY(), Lbump.getZ() );
			GLUquadric *quad1;
			quad1 = gluNewQuadric();
			gluQuadricDrawStyle( quad1, GLU_FILL);
			gluSphere( quad1 , 2.5, 10, 10);
			gluDeleteQuadric(quad1);
		}; glPopMatrix();
		
		glPushMatrix(); {
			glTranslatef( Rbump.getX(), Rbump.getY(), Rbump.getZ() );
			GLUquadric *quad2;
			quad2 = gluNewQuadric();
			gluQuadricDrawStyle( quad2, GLU_FILL);
			gluSphere( quad2 , 2.5, 10, 10);
			gluDeleteQuadric(quad2);
		}; glPopMatrix();
	}; glPopMatrix();
}

void drawFigure() {
	static double curent_time = 0;
    static double last_time = 0;
    
    last_time = curent_time;
    curent_time = (double)glutGet (GLUT_ELAPSED_TIME) / 1000.0;
	
	if (animated){
		/* Calculate current and next frames */
		Animate (&md5anim, &animInfo, curent_time - last_time);
		
		/* Interpolate skeletons between two frames */
		InterpolateSkeletons (md5anim.skelFrames[animInfo.curr_frame],
							  md5anim.skelFrames[animInfo.next_frame],
							  md5anim.num_joints,
							  animInfo.last_time * md5anim.frameRate,
							  skeleton);
	}
	else{
		/* No animation, use bind-pose skeleton */
		skeleton = md5file.baseSkel;
	}
	
	/* Draw skeleton */
	if( DISPLAY_SKELETON ){
		DrawSkeleton (skeleton, md5file.num_joints);
	}
	
	glColor3f (1.0f, 1.0f, 1.0f);
	
	if( DISPLAY_WIREFRAME ){
		glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth( 2.0f );
	} 
	else{
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth( 1.0f );
	}
	
	/* Draw each mesh of the model */
	for (int i = 0; i < md5file.num_meshes; ++i){
		md5_mesh_t mesh = md5file.meshes[i];
		
		PrepareMesh (&mesh, skeleton);
		
		DrawMesh( &mesh );
	}
}

void drawFountain() {
	glPushMatrix(); {
		float radius=1;
		float r=1;
		float h=1;
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[3]);
		float t,s;
		float i=0.05;
	
		GLUquadricObj *myQuad = gluNewQuadric();
		
		for(s=0.0;s<1.0;s+=i)
		{
			for(t=0.0;t<=1.0;t+=i)
			{       
				float r=((h-t)/h)*radius;
				glBegin(GL_QUADS); {
					glTexCoord2f(s,t);
					glVertex3f(r*cos(2*M_PI*s),t,r*sin(2*M_PI*s));
					
					glTexCoord2f(s+i,t);
					glVertex3f(r*cos(2*M_PI*(s+i)),t,r*sin(2*M_PI*(s+i)));
					
					glTexCoord2f(s+i,t+i);
					glVertex3f(r*cos(2*M_PI*(s+i)),(t+i),r*sin(2*M_PI*(s+i)));
					
					glTexCoord2f(s,t+i);
					glVertex3f(r*cos(2*M_PI*s),(t+i),r*sin(2*M_PI*s));
				};glEnd();
			}
		}
		glDisable(GL_TEXTURE_2D);
	}; glPopMatrix();
}

//generateEnvironmentDL()
//////////////////////////////////////////////////////////////
//
//	Draws all static objects: table, grid, etc.
//
////////////////////////////////////////////////////////////////////////////////
void generateEnvironmentDL() {
    environmentDL = glGenLists(1);
    glNewList(environmentDL, GL_COMPILE); {
		glPushMatrix(); {
			//glDisable( GL_LIGHTING );
			drawSky();
			//glEnable( GL_LIGHTING );
			glRotatef( -10, 0, 0, 1 );
			drawGrid();
			drawBorders();
		}; glPopMatrix();
		
		glPushMatrix(); {
			//glDisable( GL_LIGHTING );
			glEnable( GL_TEXTURE_2D );
			glBindTexture(GL_TEXTURE_2D, textures[1]);
			glRotatef( 90, 0, 1, 0 );
			glTranslatef( 16.2, -30, -45 );
			glScalef( 92, 90, 94 );
			table->draw();
			glDisable( GL_TEXTURE_2D );
			//glEnable( GL_LIGHTING );
			glUseProgram( 0 );
		}; glPopMatrix();
		
		glPushMatrix(); {
			glRotatef( -10, 0, 0, 1 );
			glTranslatef( rules[0], rules[1]+5, rules[2]);
			glRotatef( 180, 1, 0, 0 );
			glScalef( 5, 5, 5 );
			drawFountain();
		}; glPopMatrix();
	
		glPushMatrix(); {
			glRotatef( -10, 0, 0, 1 );
			glTranslatef( rules[0], rules[1], rules[2]);
			glScalef( 3, 3, 3 );
			drawFountain();
		}; glPopMatrix();
	glEnable(GL_LIGHTING);
	}; glEndList();
}

// resizeWindow() //////////////////////////////////////////////////////////////
//
//  GLUT callback for window resizing. Resets GL_PROJECTION matrix and viewport.
//
////////////////////////////////////////////////////////////////////////////////
void resizeWindow(int w, int h) {
    aspectRatio = w / (float)h;

    windowWidth = w;
    windowHeight = h;

    //update the viewport to fill the window
    glViewport(0, 0, w, h);

    //update the projection matrix with the new window properties
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,aspectRatio,0.1,100000);
}



// mouseCallback() /////////////////////////////////////////////////////////////
//
//  GLUT callback for mouse clicks. We save the state of the mouse button
//      when this is called so that we can check the status of the mouse
//      buttons inside the motion callback (whether they are up or down).
//
////////////////////////////////////////////////////////////////////////////////
void mouseCallback(int button, int state, int thisX, int thisY) {
    //update the left and right mouse button states, if applicable
    if(button == GLUT_LEFT_BUTTON)
        leftMouseButton = state;
    
    //and update the last seen X and Y coordinates of the mouse
    mouseX = thisX;
    mouseY = thisY;
}

// mouseMotion() ///////////////////////////////////////////////////////////////
//
//  GLUT callback for mouse movement. We update cameraPhi, cameraTheta, and/or
//      cameraRadius based on how much the user has moved the mouse in the
//      X or Y directions (in screen space) and whether they have held down
//      the left or right mouse buttons. If the user hasn't held down any
//      buttons, the function just updates the last seen mouse X and Y coords.
//
////////////////////////////////////////////////////////////////////////////////
void mouseMotion(int x, int y) {
	if(started && !topView && leftMouseButton == GLUT_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL) {
		globalRadius += (mouseY - y)*.05;
		camera.setRadius(globalRadius);
		camera.recomputeOrientation();
	}
    else if(started && !topView && leftMouseButton == GLUT_DOWN) {		// for the camera rotate
		globalTheta += (mouseX - x)*.005;
		globalPhi += (mouseY - y)*.005;
        camera.setTheta(globalTheta);
		camera.setPhi(globalPhi);
		camera.recomputeOrientation();
	}
	
    mouseX = x;
    mouseY = y;
}

// initScene() /////////////////////////////////////////////////////////////////
//
//  A basic scene initialization function; should be called once after the
//      OpenGL context has been created. Doesn't need to be called further.
//
////////////////////////////////////////////////////////////////////////////////
void initScene()  {
	const char *filename = "models/monsters/hellknight/mesh/hellknight.md5mesh";
	const char *animfile = "models/monsters/hellknight/animations/idle2.md5anim";
	
    glEnable(GL_DEPTH_TEST);

    // tell OpenGL not to use the material system; just use whatever we 
    // pass with glColor*()
    glEnable( GL_COLOR_MATERIAL );
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    //******************************************************************
	
	float lightCol[4] = { 1, 1, 1, 1};
    float ambientCol[4] = {1.0, 1.0, 1.0, 1.0};
    glLightfv( GL_LIGHT0,GL_DIFFUSE,lightCol );
    glLightfv( GL_LIGHT0, GL_AMBIENT, ambientCol );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
	
	float pos[4] = { 0.0, 10.0, 0.0, 1.0 };
	float newCol[4] = { 1, 1, 1, 1 };
	GLfloat dir[4] = { 0, 0, 1, 0 };
	glLightfv(GL_LIGHT7, GL_POSITION, pos);
	glLightfv(GL_LIGHT7, GL_DIFFUSE, newCol);
	glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, dir);
	glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 12);
	glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 100);
	glEnable(GL_LIGHT7);
	//******************************************************************
    glShadeModel(GL_SMOOTH);

	/* Load MD5 model file */
    if (!ReadMD5Model (filename, &md5file))
        exit (EXIT_FAILURE);
    
    AllocVertexArrays ();
    
    /* Load MD5 animation file */
    if (animfile)
    {
        if (!ReadMD5Anim (animfile, &md5anim))
        {
            FreeAnim (&md5anim);
        }
        else
        {
            animInfo.curr_frame = 0;
            animInfo.next_frame = 1;
            
            animInfo.last_time = 0;
            animInfo.max_time = 1.0 / md5anim.frameRate;
            
            /* Allocate memory for animated skeleton */
            skeleton = (struct md5_joint_t *)
            malloc (sizeof (struct md5_joint_t) * md5anim.num_joints);
            
            animated = 1;
        }
    }
    
    if (!animated) {
        printf ("[.md5anim]: no animation loaded.\n");
    }
	
    srand( time(NULL) );	// seed our random number generator
	
	//Set Up Camera
	camera = Camera();
	camera.setTheta(globalTheta);
	camera.setPhi(globalPhi);
	camera.setRadius(globalRadius);
	camera.recomputeOrientation();
}

// renderScene() ///////////////////////////////////////////////////////////////
//
//  GLUT callback for scene rendering. Sets up the modelview matrix, renders
//      a scene to the back buffer, and switches the back buffer with the
//      front buffer (what the user sees).
//
////////////////////////////////////////////////////////////////////////////////
void renderScene(void) {
	//animation variables
	int i;
	
	// clear the render buffer
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// update the modelview matrix based on the camera's position
	glMatrixMode(GL_MODELVIEW);                           // make sure we aren't changing the projection matrix!
	glLoadIdentity();
	camera.lookAt();
	
	glCallList( environmentDL );
	
	if(!started)
		drawTitle();
	
	glRotatef( -10, 0, 0, 1 );
	glPushMatrix(); {
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		pSystem.draw();
		glUseProgram( 0 );
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
	}; glPopMatrix();
	drawScore();
	//Draw everything on the table
	drawCharge();
	drawBumpers();
	for (int i = 0; i < circular_objects.size(); i++) {
		glPushMatrix(); {
			circular_objects[i].draw();
			glTranslatef( circular_objects[i].getX(), circular_objects[i].getRadius(), circular_objects[i].getZ() );
			glRotatef( -90, 1, 0, 0 );
			float scale = 0.05 + (circular_objects[i].getRadius() - 1.0 ) / 0.5 * 0.01;
			glScalef( scale, scale, scale );
			drawFigure();
		}; glPopMatrix();
	}
	gameBall.draw();
	
	//push the back buffer to the screen
    glutSwapBuffers();
}

void cleanup() {
    FreeModel (&md5file);
    FreeAnim (&md5anim);
    
    if (animated && skeleton)
    {
        free (skeleton);
        skeleton = NULL;
    }
    
    FreeVertexArrays ();
}

// keyUp() ////////////////////////////////////////////////////////////
//
//  GLUT keyboard callback; gets called when the user releases a key.
//
////////////////////////////////////////////////////////////////////////////////
void keyUp( unsigned char key, int mouseX, int mouseY ) {
	if(key == 32) {
		charging = false;
		if(gameBall.location.getX() > tableX-10 && gameBall.location.getZ() < -tableZ+4 ) {
			gameBall.direction = Vector( 1.0, 0.0, 0.0 );
			gameBall.velocity += charge*3.0;
		}
		charge = 0;
	}
	if (key == 'z' || key == 'Z') {
		LbumpOn = false;
	}
	if (key == '/') {
		RbumpOn = false;
	}
}

// normalKeysDown() ////////////////////////////////////////////////////////////
//
//  GLUT keyboard callback; gets called when the user presses a key.
//
////////////////////////////////////////////////////////////////////////////////
void normalKeysDown(unsigned char key, int x, int y) {
	//if q key is pressed
	if (key == 'q' || key == 'Q' || key == 27) {
			exit(0);
	}
		
	if(!started) {
		animating = true;
	}
	else {
		if(key == 32) {
			charging = true;
		}
		if (key == 'z' || key == 'Z') {
			LbumpOn = true;
		}
		if (key == '/') {
			RbumpOn = true;
		}
	}
}

void checkBumpers( float v ) {
	float rad = gameBall.radius;
	if(gameBall.direction.getX() > 0) {
		double temp1 = sqrt(pow(gameBall.location.getX() - Lbump.getX(), 2)
			+ pow(gameBall.location.getY() - Lbump.getY(), 2)
			+ pow(gameBall.location.getZ() - Lbump.getZ(), 2));
		double sum1 = rad + 2.5;
		if (temp1 < sum1) {
			gameBall.moveBackward();		
			Vector normal1(gameBall.location.getX() - Lbump.getX(),
				gameBall.location.getY() - Lbump.getY(),
				gameBall.location.getZ() - Lbump.getZ());
			normal1.normalize();	
			gameBall.velocity += v;
			gameBall.reflect(normal1);
		}
		
		double temp2 = sqrt(pow(gameBall.location.getX() - Rbump.getX(), 2)
			+ pow(gameBall.location.getY() - Rbump.getY(), 2)
			+ pow(gameBall.location.getZ() - Rbump.getZ(), 2));
		double sum2 = rad + 2.5;
		if (temp2 < sum1) {
			gameBall.moveBackward();		
			Vector normal2(gameBall.location.getX() - Rbump.getX(),
				gameBall.location.getY() - Rbump.getY(),
				gameBall.location.getZ() - Rbump.getZ());
			normal2.normalize();
			gameBall.velocity += v;		
			gameBall.reflect(normal2);
		}	
	}
}

// moveBall() ////////////////////////////////////////////////////////////
//
//  Handles collision detection and position updates
//
////////////////////////////////////////////////////////////////////////////////
void moveBall() {
	if(gameBall.direction.getX() < 1.0)
		gameBall.direction = gameBall.direction + Vector( .03, 0.0, 0.0 );
	if( gameBall.direction.getX() < 0 ) {
		gameBall.velocity -= 0.005;
		if(gameBall.velocity <= 0) {
			gameBall.direction = Vector(gameBall.direction.getX() * -1, gameBall.direction.getY(), gameBall.direction.getZ());
			gameBall.velocity = 0.0;
		}
	}
	else gameBall.velocity += 0.005;
	// First, move ball forward
	gameBall.moveForward();
	//Next, check if ball collides with edges of the table
	//for (unsigned int j = 0; j < balls.size(); j++) {	
	float rad = gameBall.radius;
	
	//cout << gameBall.location.getX() << " " << gameBall.location.getZ() << endl;
	
	if (gameBall.location.getX() > tableX-rad && (gameBall.location.getZ() > 5 || gameBall.location.getZ() < -5)) { // Declare vars !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		gameBall.reflect(Vector (-1, 0, 0));
	}
	else if (gameBall.location.getX() < -tableX+rad) {
		gameBall.reflect(Vector (1, 0, 0));
	}
	else if (gameBall.location.getZ() > tableZ-rad) {
		gameBall.reflect(Vector (0, 0, -1));
	}
	else if (gameBall.location.getZ() < -tableZ+rad) {
		gameBall.reflect(Vector (0, 0, 1));
	}
	else if(gameBall.location.getX() < -tableX+5+rad && gameBall.location.getZ() < -tableZ+5+rad) {
		gameBall.reflect(Vector (1, 0, 1));
	}
	else if(gameBall.location.getX() > -tableX+15 /*&& gameBall.location.getX() < tableX-20*/) {
		if( gameBall.location.getZ() < -tableZ+5 && gameBall.location.getZ() > -tableZ+2)
			gameBall.reflect(Vector(0, 0, -1));
		else if( gameBall.location.getZ() < -tableZ+8 && gameBall.location.getZ() > -tableZ+5)
			gameBall.reflect(Vector(0, 0, 1));
	}
	for(int i=23; i<=45; i++) {
		if(gameBall.location.getX() > i && gameBall.location.getZ() > 48-i) {
			gameBall.reflect(Vector(-1, 0, -1));
			break;
		}
	}
	for(int i=28; i<=45; i++) {
		if(gameBall.location.getX() > i && gameBall.location.getZ() < (-48+i) && gameBall.location.getZ() > -20){
			gameBall.reflect(Vector(-1, 0, 1));
			break;
		}
	}
	//}
	
	checkBumpers( 0 );
	
	// Check for and Handle collisions with objects with circular profiles
	for (int i = 0; i < circular_objects.size(); i++) { 
		double tempDist = sqrt(pow(gameBall.location.getX() - circular_objects[i].getX(), 2)
					+ pow(gameBall.location.getY() - circular_objects[i].getY(), 2)
					+ pow(gameBall.location.getZ() - circular_objects[i].getZ(), 2));
		double summedRadii = gameBall.radius + circular_objects[i].getRadius();
		
		if (tempDist < summedRadii) {
			gameBall.moveBackward();
					
			Vector normal_ji(gameBall.location.getX() - circular_objects[i].getX(), gameBall.location.getY() - circular_objects[i].getY(), gameBall.location.getZ() - circular_objects[i].getZ());
			normal_ji.normalize();
			gameBall.velocity += 2;			
			gameBall.reflect(normal_ji);
			score+=10;
		}
	}
		
	// Check for and handle collisions with objects with rectangular profiles
	for (int i = 0; i < rectangular_objects.size(); i++) {
		if (abs(gameBall.location.getX() - rectangular_objects[i].getX()) < gameBall.radius) {
			gameBall.moveBackward();
			Vector tempNormal(-1, 0, 0);
			Vector outVector = gameBall.direction - (2 * dot(gameBall.direction, tempNormal)) * tempNormal;
			outVector.normalize();
			gameBall.direction = outVector;
			gameBall.moveForward();	
		}
		else if (abs(gameBall.location.getX() - (rectangular_objects[i].getX() + rectangular_objects[i].getDeltaX())) < gameBall.radius) {
			gameBall.moveBackward();
			Vector tempNormal(1, 0, 0);
			Vector outVector = gameBall.direction - (2 * dot(gameBall.direction, tempNormal)) * tempNormal;
			outVector.normalize();
			gameBall.direction = outVector;
			gameBall.moveForward();
		}
		else if (abs(gameBall.location.getZ() - rectangular_objects[i].getZ()) < gameBall.radius) {
			gameBall.moveBackward();
			Vector tempNormal(0, 0, -1);
			Vector outVector = gameBall.direction - (2 * dot(gameBall.direction, tempNormal)) * tempNormal;
			outVector.normalize();
			gameBall.direction = outVector;
			gameBall.moveForward();	
		}
		else if (abs(gameBall.location.getZ() - (rectangular_objects[i].getZ() + rectangular_objects[i].getDeltaZ())) < gameBall.radius) {
			gameBall.moveBackward();
			Vector tempNormal(0, 0, 1);
			Vector outVector = gameBall.direction - (2 * dot(gameBall.direction, tempNormal)) * tempNormal;
			outVector.normalize();
			gameBall.direction = outVector;
			gameBall.moveForward();
		}
	}
	
	if(gameBall.location.getX() > tableX)
		initialize();
}
// myTimer() ////////////////////////////////////////////////////////////////////
//
//  GLUT timer callback; gets called when a timer expires
//
////////////////////////////////////////////////////////////////////////////////
void myTimer( int value ) {
	if(animating) {
		globalRadius -=0.75;
		if(globalRadius <= 110) {
			globalRadius = 110;
			animating = false;
			started = true;
		}
		camera.setRadius(globalRadius);
		camera.recomputeOrientation();
	}
	
	if(charging) {
		charge+= 0.02;
		if(charge > 3.0)
			charge=3.0;
	}
	
	if(LbumpOn && Lbump.getX() > 39) {
		Lbump += Vector( -0.7, 0.0, -0.7);
		checkBumpers( 4 );
	}
	else if( !LbumpOn && Lbump.getX() < 42.5) {
		Lbump += Vector( 0.7, 0.0, 0.7);
		checkBumpers( 0 );
	}
	if(RbumpOn && Rbump.getX() > 39) {
		Rbump += Vector( -0.7, 0.0, 0.7);
		checkBumpers( 4 );
	}
	else if( !RbumpOn && Rbump.getX() < 42.5) {
		Rbump += Vector( 0.7, 0.0, -0.7);
		checkBumpers( 0 );
	}
	
	if (ballEnabled) {
		moveBall();
	}
	// End of collision detection and handling

	//Particle System
	Vector a = Vector(0, 0, 1);
	Vector b = Vector( camera.getX(), camera.getY(), camera.getZ() );
    Vector c = cross( a, b );
	a.normalize();
	b.normalize();
	double dd = acos(dot( a, b )) * 180 / M_PI;
	
	glPushMatrix(); {
	if(getRand() < rules[8]/10 && fountainOn)
		pSystem.addParticle();
		pSystem.changeRot( dd, c );
		pSystem.update();
	}; glPopMatrix();
	
	// redraw our display
    glutPostRedisplay();
	glutSetWindow( winMain );
    // register a new timer callback
    glutTimerFunc( 1000.0f / 60.0f, myTimer, 0 );
}

// myMenu() /////////////////////////////////////////////////////////////////////
//
//  Handles our Menu events
//
////////////////////////////////////////////////////////////////////////////////
void myMenu( int value ) {
	if (value == 0) {
		exit(0);
	}
	if (started && value == 1) {
		if(topView) {
			camera.setTheta(globalTheta);
			camera.setPhi(globalPhi);
			camera.setRadius(globalRadius);
		}
		else {
			camera.setTheta(M_PI / 2.0f);
			camera.setPhi(M_PI / 1.05f);
			camera.setRadius(125);
		}
		topView = !topView;
		camera.recomputeOrientation();
	}
}

// createMenus() ///////////////////////////////////////////////////////////////
//
//  Handles creating a menu, adding menu entries, and attaching the menu to
//  a mouse button
//
////////////////////////////////////////////////////////////////////////////////
void createMenus() {
	int menuId = glutCreateMenu(myMenu);
	glutAddMenuEntry("Quit", 0);
	glutAddMenuEntry("Switch Camera", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// registerCallbacks() /////////////////////////////////////////////////////////
//
//  Register all of our callbacks for GLUT.
//
////////////////////////////////////////////////////////////////////////////////
void registerCallbacks() {
    // keyboard callbacks
    glutSetKeyRepeat(   GLUT_KEY_REPEAT_ON );
    glutKeyboardFunc(   normalKeysDown     );
	glutKeyboardUpFunc(		keyUp		);

    // mouse callbacks
    glutMouseFunc(      mouseCallback      );
    glutMotionFunc(     mouseMotion        );

    // display callbacks
    glutDisplayFunc(    renderScene        );
    glutReshapeFunc(    resizeWindow       );

    // timer callback
    glutTimerFunc( 1000.0f / 60.0f, myTimer, 0 );
}


void loadParticleRules( char* filename ) {
	fstream file;
	file.open(filename);
	string currentLine;
	if (file.is_open()) {
		while (getline(file, currentLine)) {
			if(currentLine.at(0) != '#') {
				currentLine.erase(0, currentLine.find(',', 0)+1);
				for(int i=0; i<9; i++) {
					int length = currentLine.find(',', 0);
					string first = currentLine.substr(0, length);
					rules[i] = atof(first.c_str());
					if(i < 8)
						currentLine.erase(0, length+1);
				}
			}
		}
		file.close();
	}
}

// main() //////////////////////////////////////////////////////////////////////
//
//  Program entry point. Takes a single command line argument for our 
//    control points file.
//
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv ) {
    loadParticleRules( "particleRules.txt" );
	glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize( windowWidth, windowHeight );
    winMain = glutCreateWindow( "Pinball" );
	
	GLenum glewResult = glewInit();
    /* check for an error */
    if( glewResult != GLEW_OK ) {
        printf( "[ERROR]: Error initalizing GLEW\n");
        return 0;
    }
    /* make sure OpenGL 2.0 is supported */
    if( !glewIsSupported( "GL_VERSION_2_0" ) ) {
        printf( "[ERROR]: System does not support OpenGL 2.0 and GLSL\n" );
        return 0;
    }
	
    fprintf(stdout, "[INFO]: /-------------------------------------------------------------\\\n");
    fprintf(stdout, "[INFO]: | OpenGL Information                                          |\n");
    fprintf(stdout, "[INFO]: |-------------------------------------------------------------|\n");
    fprintf(stdout, "[INFO]: |   OpenGL Version:  %40s |\n", glGetString(GL_VERSION));
    fprintf(stdout, "[INFO]: |   OpenGL Renderer: %40s |\n", glGetString(GL_RENDERER));
    fprintf(stdout, "[INFO]: |   OpenGL Vendor:   %40s |\n", glGetString(GL_VENDOR));
    fprintf(stdout, "[INFO]: |   GLUI Version:    %40.2f |\n", GLUI_VERSION);
    fprintf(stdout, "[INFO]: \\-------------------------------------------------------------/\n");	
    //clean up the animation stuff
	atexit (cleanup);
	// do some basic OpenGL setup
    initScene();

    // create our menu options and attach to mouse button
    createMenus();
	
    // register callback functions...
    registerCallbacks();
	
	//Load Textures
	textures[0] = SOIL_load_OGL_texture(
		"textures/skybox.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_INVERT_Y
		| SOIL_FLAG_COMPRESS_TO_DXT );
		
	textures[1] = SOIL_load_OGL_texture(
		"textures/table_skin.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_INVERT_Y
		| SOIL_FLAG_COMPRESS_TO_DXT );
		
	textures[2] = SOIL_load_OGL_texture(
		"textures/table_skin3.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_INVERT_Y
		| SOIL_FLAG_COMPRESS_TO_DXT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	
		
	textures[3] = SOIL_load_OGL_texture(
		"textures/charge.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_INVERT_Y
		| SOIL_FLAG_COMPRESS_TO_DXT );
		
	textures[4] = SOIL_load_OGL_texture(
		"textures/firework1.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_INVERT_Y
		| SOIL_FLAG_COMPRESS_TO_DXT );
		
	textures[5] = SOIL_load_OGL_texture(
		"textures/firework2.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_INVERT_Y
		| SOIL_FLAG_COMPRESS_TO_DXT );
		
	textures[6] = SOIL_load_OGL_texture(
		"textures/firework3.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_INVERT_Y
		| SOIL_FLAG_COMPRESS_TO_DXT );
	
	textures[7] = SOIL_load_OGL_texture(
		"textures/firework4.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_INVERT_Y
		| SOIL_FLAG_COMPRESS_TO_DXT );
	
	table = new Object( "table.obj" );
	
	GLuint texs[4];
	for(int i=0; i<4; i++) {
		texs[i] = textures[i+4];
	}
	
	Shader shade = Shader();
	particleShaderHandle = shade.setupShaders( "passThrough.v.glsl", "passThrough.f.glsl" );
	pSystem = ParticleSystem( rules, particleShaderHandle, texs);

	
	// Initialize gameBall
	initialize();
	
	// Temporary initialization; actual initialization will be done with board data
	circular_objects.push_back(CircularBoardObject(0, 0, 0, 2.0));
	//circular_objects.push_back(CircularBoardObject(10, 0, 10, 2.5));
	//circular_objects.push_back(CircularBoardObject(-10, 0, -10, 3.0));
	//RectangularBoardObject tRBO(8, 0, 8, 4, 4);
	//rectangular_objects.push_back(tRBO);
	started = false;
	animating = false;
    generateEnvironmentDL();
	
	// and enter the GLUT loop, never to exit.
    glutMainLoop();

    return(0);
}
