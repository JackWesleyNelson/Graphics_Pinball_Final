const vec4 red = vec4(1.0, 0.0, 0.0, 1.0);

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
} 