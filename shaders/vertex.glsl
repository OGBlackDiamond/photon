#version 460 core
layout (location = 0) in vec3 aPos;

out vec2 pixelPosition;

void main() {
    gl_Position = vec4(aPos, 1.0f);
    pixelPosition = gl_Position.xy;
}
