#include "Shader.h"

Shader::Shader() {
	
}

void Shader::readTextFromFile( string filename, char* &output ){
    string buf = string("");
    string line;
    
    ifstream in(filename.c_str());
    while( getline(in, line) ) {
        buf += line + "\n";
    }
    output = new char[buf.length()+1];
    strncpy(output, buf.c_str(), buf.length());
    output[buf.length()] = '\0';
    
    in.close();
}

void Shader::printLog( GLuint handle ) {
    int infologLength = 0;
    int maxLength;
    
    /* check if the handle is to a vertex/fragment shader */
    if( glIsShader( handle ) ) {
        glGetShaderiv(  handle, GL_INFO_LOG_LENGTH, &maxLength );
    }
    /* check if the handle is to a shader program */
    else {
        glGetProgramiv( handle, GL_INFO_LOG_LENGTH, &maxLength );
    }
    
    /* create a buffer of designated length */
    char infoLog[maxLength];
    
    bool isShader;
    if( glIsShader( handle ) ) {
        /* get the info log for the vertex/fragment shader */
        glGetShaderInfoLog(  handle, maxLength, &infologLength, infoLog );
        
        isShader = true;
    } else {
        /* get the info log for the shader program */
        glGetProgramInfoLog( handle, maxLength, &infologLength, infoLog );
        
        isShader = false;
    }
    
    /* if the length of the log is greater than 0 */
    if( infologLength > 0 ) {
        /* print info to terminal */
        printf( "[INFO]: %s Handle %d: %s\n", (isShader ? "Shader" : "Program"), handle, infoLog );
    }
}

GLuint Shader::compileShader( string filename, GLenum shaderType ) {
    char *shaderString;
    
    /* read in each text file and store the contents in a string */
    readTextFromFile( filename.c_str(), shaderString );
    
    /* create a handle to our shader */
    GLuint shaderHandle = glCreateShader( shaderType );
    
    /* send the contents of each program to the GPU */
    glShaderSource( shaderHandle, 1, (const char**)&shaderString, NULL );	printLog( shaderHandle );
    
    /* we are good programmers so free up the memory used by each buffer */
    delete [] shaderString;
    
    /* compile each shader on the GPU */
    glCompileShader( shaderHandle );	printLog( shaderHandle );
    
    return shaderHandle;
}

GLuint Shader::setupShaders( char *vertexShaderFilename, char *fragmentShaderFilename ) {
    
    /* compile each one of our shaders */
    GLuint vertexShaderHandle   = compileShader( vertexShaderFilename,   GL_VERTEX_SHADER   );
    GLuint fragmentShaderHandle = compileShader( fragmentShaderFilename, GL_FRAGMENT_SHADER );
    
    /* get a handle to a shader program */
    GLuint shaderProgramHandle = glCreateProgram();
    
    /* attach the vertex, geometry, and fragment shaders to the shader program */
    glAttachShader( shaderProgramHandle, vertexShaderHandle   );	printLog( vertexShaderHandle   );
    glAttachShader( shaderProgramHandle, fragmentShaderHandle );	printLog( fragmentShaderHandle );
    
    /* link all the programs together on the GPU */
    glLinkProgram( shaderProgramHandle );	printLog( shaderProgramHandle );
    
    /* return handle */
    return shaderProgramHandle;
}