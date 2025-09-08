#version 460 core

out vec4 FragColor;

uniform vec4 color; // Uniform to set the color from C++ code

void main() { FragColor = color; }