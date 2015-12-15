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

#ifndef _SHADER_H_
#define _SHADER_H_

class Shader {
public:

	Shader();
	
	void readTextFromFile( string filename, char* &output );
	void printLog( GLuint handle );
	GLuint compileShader( string filename, GLenum shaderType );
	GLuint setupShaders( char *vertexShaderFilename, char *fragmentShaderFilename );
	
private:

};
#endif