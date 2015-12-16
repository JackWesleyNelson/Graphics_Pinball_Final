varying vec3 eyeDir;
varying vec3 normal;
uniform samplerCube cubeMap;

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	normal = gl_NormalMatrix * gl_Normal;
	eyeDir = vec3(gl_ModelViewMatrix * gl_Vertex);
} 