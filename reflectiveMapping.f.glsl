uniform samplerCube cubeMap;
varying vec3 reflect;

void main() {
	vec4 outputColor = vec4(textureCube(cubeMap, reflect));
	
	gl_FragColor = clamp(outputColor, 0.0, 1.0);
} 