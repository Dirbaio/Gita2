#version 120

uniform mat4 modelViewProjectionMatrix;

attribute vec3 a_position;
attribute vec3 a_normal;

varying vec4 vPos;
varying vec3 vPos2;
varying vec3 vNormal;
void main () {
    gl_Position = modelViewProjectionMatrix * vec4(a_position,1.0);
    vPos =  modelViewProjectionMatrix * vec4(a_position,1.0);
    vPos2 = a_position;
    vNormal = normalize(a_normal);
}
