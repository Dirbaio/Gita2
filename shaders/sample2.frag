#version 120

varying vec3 vColor;

void main() {
    float m = max(vColor.r, max(vColor.g, vColor.b));
    gl_FragColor = vec4(floor((vColor/m)*8.5)/8.0, 1.0);
}
