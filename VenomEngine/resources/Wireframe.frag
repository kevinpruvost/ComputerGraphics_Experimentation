#version 330 core

out vec4 fragColor;
uniform vec3 wireframeColor;

void main() {
    fragColor = vec4(wireframeColor, 1.0); // White color for wireframe
}