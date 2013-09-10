#version 120

uniform sampler2D tex;
varying vec2 vTexCoord;

void main() {
    gl_FragColor = texture2D(tex, vTexCoord);
}
