#include "ball.h"

Ball::Ball(float radius, std::string ballname, double g, glm::vec2 position,
           int segments)
    : radius(radius), name(ballname), gravity(g), position(position), VAO(0),
      VBO(0), vertex_count(segments + 2) {
  setupMesh();
}

Ball::~Ball() {
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
}
void Ball::setupMesh() {
  std::vector<float> vertices;

  // Center (initial) of the ball
  vertices.push_back(position.x);
  vertices.push_back(position.y);

  // 2π / segments
  float angle_step = (2.0f * glm::pi<float>()) / (vertex_count - 2);

  for (int i = 0; i < vertex_count - 2; i++) {
    float angle = angle_step * i;
    float x = position.x + radius * cos(angle);
    float y = position.y + radius * sin(angle);
    vertices.push_back(x);
    vertices.push_back(y);
  }

  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);

  // State machine binding for some reason
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // GL_STATIC_DRAW -> the geometry of the ball never changes but position does
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}