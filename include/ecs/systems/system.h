#pragma once

#include "component.h"
#include "pch.h"

namespace ecs {
namespace sys {
class Physics {
public:
  /**
   * Integrates velocities and positions for all physics-enabled entities.
   *
   * @param {delta} delta time.
   * @param {transforms} map from Entity -> ecs::comp::Transform (modified in
   * place).
   * @param {physics} map from Entity -> ecs::comp::Physics (modified in
   * place).
   * @param {floorY} world bottom (in the same units as transform.position.y,
   *                   typically window height in pixels).
   *
   * Integration details:
   *  - Uses explicit Euler integration:
   *      v <- v + a * dt
   *      x <- x + v * dt
   *    This is simple and matches the old Ball::transform behaviour.
   *  - After integrating position, checks if entity is considered "below"
   *    the floor.
   *    If (position.y + (2 * scale) > floorY) we mark it for removal.
   *
   * @returns vector<Entity> a list of entities that are out-of-bounds and
   * should be removed/cleaned up by the caller.
   */
  std::vector<Entity>
  update(float delta,
         std::unordered_map<Entity, ecs::comp::Transform> &transforms,
         std::unordered_map<Entity, ecs::comp::Physics> &physics,
         float floorY);
};

class Render {
  unsigned int shaderProgram;
  glm::mat4 projection, view;

  GLint modelLoc{-1}, viewLoc{-1}, projectionLoc{-1}, colorLoc{-1};

public:
  /**
   * Construct a RenderSystem.
   *
   * @param {shader} OpenGL shader program (GLuint). The program must have the
   *                 uniforms: "model", "view", "projection", and "color".
   * @param {proj} projection matrix (e.g. orthographic projection).
   * @param {v} view matrix (camera transform).
   *
   * The constructor caches uniform locations so draw() doesn't call
   * glGetUniformLocation per object.
   */
  Render(unsigned int shader, const glm::mat4 &proj, const glm::mat4 &v);

  /**
   * Draw all renderable entities.
   *
   * @param {transforms} map from Entity -> ecs::comp::Transform (read-only).
   * @param {renderables} map from Entity -> ecs::comp::Render (read-only).
   *
   * For each entity present in renderables, this method will:
   *  1. find its ecs::comp::Transform (skip if missing),
   *  2. build a model matrix from position + scale (translation then scale),
   *  3. upload model/view/projection + color uniforms,
   *  4. bind the entity VAO and call glDrawArrays(GL_TRIANGLE_FAN, ...).
   *
   * Notes:
   *  - renderables with VAO == 0 or vertex_count <= 0 will be skipped.
   *  - uniform locations are cached on construction; if your shader changes,
   *    create a new RenderSystem or expose a setter to refresh the locations.
   */
  void draw(const std::unordered_map<Entity, ecs::comp::Transform> &transforms,
            const std::unordered_map<Entity, ecs::comp::Render> &renderables);
};
} // namespace sys
} // namespace ecs