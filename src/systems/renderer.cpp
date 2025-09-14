#include "system.h"

ecs::sys::Render::Render(unsigned int shader, const glm::mat4 &proj,
                           const glm::mat4 &v)
    : shaderProgram(shader), projection(proj), view(v) {
  glUseProgram(shaderProgram);
  modelLoc = glGetUniformLocation(shaderProgram, "model");
  viewLoc = glGetUniformLocation(shaderProgram, "view");
  projectionLoc = glGetUniformLocation(shaderProgram, "projection");
  colorLoc = glGetUniformLocation(shaderProgram, "color");
  glUseProgram(0);
}

void ecs::sys::Render::draw(
    const std::unordered_map<Entity, ecs::comp::Transform> &transforms,
    const std::unordered_map<Entity, ecs::comp::Render> &renderables) {
  // Bind the shared shader program once per draw call.
  glUseProgram(shaderProgram);

  // Upload view & projection (same for all objects in this pass).
  if (viewLoc != -1) {
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  }
  if (projectionLoc != -1) {
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
  }

  // Draw every renderable that also has a transform
  for (const auto &pair : renderables) {
    const Entity entity = pair.first;
    const ecs::comp::Render &r = pair.second;

    // Skip invalid geometry
    if (r.VAO == 0 || r.vertex_count <= 0)
      continue;

    // Find the transform
    auto it = transforms.find(entity);
    if (it == transforms.end())
      continue;
    const ecs::comp::Transform &t = it->second;

    // Build model matrix: translate -> scale
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(t.position, 0.0f));
    model = glm::scale(model, glm::vec3(t.scale, t.scale, 1.0f));

    // Upload model matrix
    if (modelLoc != -1) {
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    }

    // Upload color (if the shader has a color uniform)
    glm::vec4 colors{r.color.r, r.color.g, r.color.b, r.color.a};
    if (colorLoc != -1) {
      glUniform4fv(colorLoc, 1, glm::value_ptr(colors));
    }

    // Bind VAO and draw
    glBindVertexArray(r.VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, r.vertex_count);
    glBindVertexArray(0);
  }

  // Unbind shader
  glUseProgram(0);
}