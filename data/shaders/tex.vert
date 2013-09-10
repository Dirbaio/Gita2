#version 120

uniform mat4 modelViewProjectionMatrix;
uniform vec4 texBounds;

attribute vec3 a_position;
attribute vec2 a_texCoord;

varying vec2 vTexCoord;

void main () {
    gl_Position = modelViewProjectionMatrix * vec4(a_position,1.0);
    vTexCoord = texBounds.xy + texBounds.zw*a_texCoord;
}
