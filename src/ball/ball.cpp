#include "ball.h"

Ball::Ball(float radius, int segments, std::string ballname, glm::vec2 position,
           Color color, glm::vec2 g)
    : radius(radius), name(ballname), gravity(g), position(position), VAO(0),
      VBO(0), vertex_count(segments + 2), velocity(0), color(color) {
  setupMesh();
}

Ball::~Ball() {
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
}

void Ball::setupMesh() {
  std::vector<float> vertices;

  // Center of the ball (0, 0) because it's relative to the shape drawn
  vertices.push_back(0.0f);
  vertices.push_back(0.0f);

  // 2π / segments
  float angle_step = (2.0f * glm::pi<float>()) / (vertex_count - 2);

  for (int i = 0; i <= vertex_count - 1; i++) {
    float angle = angle_step * i;
    float x = radius * cos(angle);
    float y = radius * sin(angle);
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
  glBindVertexArray(0); // unbinds
}

void Ball::draw(unsigned int shaderProgram, const glm::mat4 &projection,
                const glm::mat4 &view) {
  glUseProgram(shaderProgram);

  // pass model matrix, translate ball to current position
  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
  unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

  // pass view matrix
  unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

  // pass projection matrix
  unsigned int projectionLoc =
      glGetUniformLocation(shaderProgram, "projection");
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

  // pass colors
  glm::vec4 colors{color.r, color.g, color.b, color.a};
  unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");
  glUniform4fv(colorLoc, 1, glm::value_ptr(colors));

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_FAN, 0, vertex_count);
  glBindVertexArray(0); // unbinds
}

bool Ball::transform(float delta) {
  velocity += gravity * delta;  // v = v + at
  position += velocity * delta; // p = p + vt
  if (position.y + (2 * radius) > 1080.0f) {
    return false;
  }
  return true;
}