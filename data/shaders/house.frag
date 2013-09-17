#version 120

varying vec4 vPos;
varying vec3 vPos2;
varying vec3 vNormal;

void main() {
    vec3 lightDir = normalize(vec3(-1, -2, -2));
    float light = abs(dot(lightDir, vNormal));
    float yy = vPos2.y;
    light *= 1-exp(-yy*0.3)+0.4;

    float alpha = max(0.5, 1-exp(-length(vPos.xy)*0.6));
    if(vNormal.y < 0.5) alpha = 1;
    gl_FragColor = vec4(light, light, light, alpha);
}
