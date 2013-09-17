#version 120

uniform mat4 modelViewProjectionMatrix;

attribute vec3 a_position;

void main () {
    gl_Position = modelViewProjectionMatrix * vec4(a_position,1.0);
}
