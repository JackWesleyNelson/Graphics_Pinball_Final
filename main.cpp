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


// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

static size_t windowWidth = 640;
static size_t windowHeight = 480;
static float aspectRatio;

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

//Textures
GLuint textures[2];

//Game Variables
bool started = false, animating = false;

// getRand() ///////////////////////////////////////////////////////////////////
//
//  Simple helper function to return a random number between 0.0f and 1.0f.
//
////////////////////////////////////////////////////////////////////////////////
float getRand() {
    return rand() / (float)RAND_MAX;
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
			for (int i = -50; i <= 50; i +=5)
				for (int j = -25; j <= 25; j +=5) {
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

//drawSky()
//////////////////////////////////////////////////////////////
//
//	Draws the skybox
//
////////////////////////////////////////////////////////////////////////////////
void drawSky() {
	glPushMatrix(); {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		
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
		}; glPopMatrix();
		
		glPushMatrix(); {
			//glDisable( GL_LIGHTING );
			glEnable( GL_TEXTURE_2D );
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			glRotatef( 90, 0, 1, 0 );
			glTranslatef( 16, -30, -45 );
			glScalef( 90, 90, 92 );
			table->draw();
			glDisable( GL_TEXTURE_2D );
			//glEnable( GL_LIGHTING );
			glUseProgram( 0 );
		}; glPopMatrix();
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
	//******************************************************************
    glShadeModel(GL_SMOOTH);

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
	
	//push the back buffer to the screen
    glutSwapBuffers();
}

// keyUp() ////////////////////////////////////////////////////////////
//
//  GLUT keyboard callback; gets called when the user releases a key.
//
////////////////////////////////////////////////////////////////////////////////
void keyUp( unsigned char key, int mouseX, int mouseY ) {
	
}

// normalKeysDown() ////////////////////////////////////////////////////////////
//
//  GLUT keyboard callback; gets called when the user presses a key.
//
////////////////////////////////////////////////////////////////////////////////
void normalKeysDown(unsigned char key, int x, int y) {
	//if q key is pressed
	if(!started) {
		animating = true;
	}
	else {
		if (key == 'q' || key == 'Q' || key == 27) {
			exit(0);
		}
	}
}

// myTimer() ////////////////////////////////////////////////////////////////////
//
//  GLUT timer callback; gets called when a timer expires
//
////////////////////////////////////////////////////////////////////////////////
void myTimer( int value ) {
	if(animating) {
		globalRadius -=0.75;
		if(globalRadius <= 100) {
			globalRadius = 100;
			animating = false;
			started = true;
		}
		camera.setRadius(globalRadius);
		camera.recomputeOrientation();
	}

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

// main() //////////////////////////////////////////////////////////////////////
//
//  Program entry point. Takes a single command line argument for our 
//    control points file.
//
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv ) {
    
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
	
    // do some basic OpenGL setup
    initScene();

    // create our menu options and attach to mouse button
    createMenus();

    // register callback functions...
    registerCallbacks();
	
	//Load Textures
	textures[0] = SOIL_load_OGL_texture(
		"textures/table_skin.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_INVERT_Y
		| SOIL_FLAG_COMPRESS_TO_DXT );
		
	textures[1] = SOIL_load_OGL_texture(
		"textures/skybox.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_INVERT_Y
		| SOIL_FLAG_COMPRESS_TO_DXT );
	
	table = new Object( "table.obj" );
	
    generateEnvironmentDL();

	// and enter the GLUT loop, never to exit.
    glutMainLoop();

    return(0);
}
