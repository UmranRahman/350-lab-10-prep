#version 120 // glsl version 1.2 for OpenGL 2.1


uniform sampler2D texture;
uniform vec2 windowSize;

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec2 texel = 1.0 / windowSize;

    vec4 sum = vec4(0.0);

    for (int dx = -2; dx <= 2; dx++) {
        for (int dy = -2; dy <= 2; dy++) {
            vec2 offset = vec2(float(dx), float(dy)) * texel;
            sum += texture2D(texture, uv + offset);
        }
    }

    gl_FragColor = sum / 25.0;
}
