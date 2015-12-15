varying vec3 reflection;

void main() {
	gl_Position = ftransform();
	vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
	vec4 pos = gl_ModelViewMatrix * gl_Vertex;
	vec3 eye = pos.xyz;
	reflection = reflect(eye, normal);
} 