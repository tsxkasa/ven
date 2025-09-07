#version 460 core

in vec3 fragColor;
out vec4 FragColor;

uniform vec4 color; 

void main() {
  FragColor = color; // RGBA
}