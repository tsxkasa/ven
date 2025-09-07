#pragma once

#include "pch.h"

class Ball {
private:
  float radius;
  std::string name;
  double gravity;
  glm::vec2 position;

  unsigned int VAO, VBO;
  int vertex_count;

  void setupMesh();

public:
  /*
  @param {radius} Object radius
  @param {ballname} Object name
  @param {g} Object gravity (defaults to 9.80665, Earth's surface gravity)
  @param {position} Object initial position in {x, y} (defaults to {0, 0})
  @param {segments} Determines how smooth the ball appears; "edges" of the ball
  */
  Ball(float radius, std::string ballname, double g = 9.80665,
       glm::vec2 position = {0.0f, 0.0f}, int segments);
  ~Ball();
};