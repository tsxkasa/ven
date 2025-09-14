#include "ecs.h"

ecs::management::ResourceControl::ResourceControl(unsigned int shader,
                                                  const glm::mat4 &proj,
                                                  const glm::mat4 &v)
    : render_system(shader, proj, v) {}