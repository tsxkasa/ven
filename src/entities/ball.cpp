#include "entity.h"

Entity ecs::ent::createBall(std::unordered_map<Entity, ecs::comp::Transform> &transforms,
                  std::unordered_map<Entity, ecs::comp::Physics> &physics,
                  std::unordered_map<Entity, ecs::comp::Render> &renderables,
                  float radius, int segments, glm::vec2 position,
                  Color color, glm::vec2 g) {
  static Entity nextId = 1;
  Entity entity = nextId++;

  // Transform
  transforms[entity] = {
      position,     // pos
      {0.0f, 0.0f}, // rotation
      1.0f        // use radius as scale
  };

  // Physics
  physics[entity] = {
      1.0f,         // mass
      {0.0f, 0.0f}, // velocity
      g             // gravity
  };

  // Render
  ecs::comp::Render render;
  render.vertex_count = segments + 2;
  render.color = color;

  // --- Setup geometry ---
  std::vector<float> vertices;
  vertices.push_back(0.0f);
  vertices.push_back(0.0f);

  float angle_step = (2.0f * glm::pi<float>()) / (render.vertex_count - 2);

  for (int i = 0; i <= render.vertex_count - 1; i++) {
    float angle = angle_step * i;
    float x = radius * cos(angle);
    float y = radius * sin(angle);
    vertices.push_back(x);
    vertices.push_back(y);
  }

  glGenBuffers(1, &render.VBO);
  glGenVertexArrays(1, &render.VAO);

  glBindVertexArray(render.VAO);
  glBindBuffer(GL_ARRAY_BUFFER, render.VBO);

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  renderables[entity] = render;

  return entity;
}