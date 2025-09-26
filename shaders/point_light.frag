#version 460

layout(location = 0) in vec2 fragOffset;
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUBO {
  mat4 projection;
  mat4 view;
  vec4 ambientLightColor;
  vec3 lightPosition;
  vec4 lightColor;
}
UBO;

void main() {
  float dis = sqrt(dot(fragOffset, fragOffset));
  if (dis >= 1.0)
    discard;
  outColor = vec4(UBO.lightColor.xyz, 1.0);
}