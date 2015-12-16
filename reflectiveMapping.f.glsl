uniform samplerCube cubeMap;

varying vec3 normal;
varying vec3 eyeDir;

void main() {
	vec3 reflectedDirection = normalize(reflect(eyeDir, normalize(normal)));
	//reflectedDirection.y = -reflectedDirection.y;
	vec4 temp_FG = textureCube(cubeMap, reflectedDirection);
	gl_FragColor = temp_FG;
} 