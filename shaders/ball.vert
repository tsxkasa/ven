#version 460 core

layout(location = 0)
    in vec2 aPos; // Change to vec2 because your C++ code uses 2D positions

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() { gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0); }