#pragma once

#include "pch.h"

// distance object fall over t time is (1/2)gt^2
class Ball {
private:
  float radius;
  std::string name;
  double gravity;
  glm::vec2 position;

  unsigned int VAO, VBO;
  int vertex_count;
  Color color;

  // Initializes VAO/VBO
  void setupMesh();

public:
  /**
  Constructs a ball object
  * @param {radius} Object radius
  * @param {segments} Determines how smooth the ball appears; "edges" of the
  ball
  * @param {ballname} Object name
  * @param {position} Object initial position in {x, y} (defaults to {0, 0})
  * @param {color} Color of the ball (1 color per ball)
  * @param {g} Object gravity (defaults to 9.80665, Earth's surface gravity)
  */
  Ball(float radius, int segments, std::string ballname,
       glm::vec2 position = {0.0f, 0.0f},
       Color color = {1.0f, 1.0f, 1.0f, 1.0f}, double g = 9.80665);
  ~Ball();

  /**
   * Draws the ball onto window pane
   * @param {shaderProgram} Initialized shader program object
   * @param {projection} Field of view/space projection (orthographic projection
   * in 2D space)
   * @param {view} Camera view (could be identity matrix in 2D space)
   */
  void draw(unsigned int shaderProgram, const glm::mat4 &projection,
            const glm::mat4 &view);

  /**
   * Transform the ball Δ pixel from center
   * @param {delta} Δpos in pixel
   */
  void transform(glm::vec2 delta);
};