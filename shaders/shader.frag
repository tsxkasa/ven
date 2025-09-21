#version 460 core

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
  vec2 offset;
  vec3 color;
}
push;

// #664f8C (0.4, 0.309, 0.549, 1.0)
void main() {
  outColor = vec4(push.color, 1.0); 
}