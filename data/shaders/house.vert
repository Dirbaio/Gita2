#version 120

uniform mat4 modelViewProjectionMatrix;

attribute vec3 a_position;
attribute vec3 a_normal;

varying vec3 vColor;

void main () {
	gl_Position = modelViewProjectionMatrix * vec4(a_position,1.0);
	vColor = abs(a_normal);
}
