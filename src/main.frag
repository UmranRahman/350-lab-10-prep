#version 120 // glsl version 1.2 for OpenGL 2.1

uniform vec2 windowSize;
uniform vec2 mousePos;
uniform float time;

void main(void) {
    vec2 uv = gl_FragCoord.xy / windowSize;

    float shift = time / 4000.0;
    float x = fract(uv.x + shift);

    // Fragment shader should output into gl_FragColor an RGBA:
    vec4 color = vec4(x, uv.y, 0.0, 1.0);


    float dist = distance(gl_FragCoord.xy, mousePos);

    if (dist < 10.0) {
        color = vec4(1.0, 1.0, 1.0, 1.0);
    }

    gl_FragColor = color;
}
