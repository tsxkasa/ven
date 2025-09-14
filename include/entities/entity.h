#pragma once

#include "component.h"
#include "pch.h"

namespace ecs {
namespace ent {
/**
 * Factory function that creates a "Ball" entity in the ECS.
 *
 * A Ball is defined as an entity that has a ecs::comp::Transform,
 * ecs::comp::Physics, and ecs::comp::Render.
 *
 * @param {transforms} map of Entity -> ecs::comp::Transform. This will be
 *        populated with the new entity's transform (position, rotation, scale).
 *        - position is initialized from the provided argument.
 *        - scale is set to the radius of the ball.
 * @param {physics} map of Entity -> ecs::comp::Physics. This will be populated
 *        with the new entity's physics (mass, velocity, gravity).
 *        - mass is set to 1.0 by default.
 *        - velocity starts at (0,0).
 *        - gravity is set from the argument (default Earth's 9.80665 m/s²).
 * @param {renderables} map of Entity -> ecs::comp::Render. This will be
 *        populated with the new entity's render data (VAO, VBO, vertices,
 * color).
 *        - vertex_count = segments + 2 (center vertex + ring).
 *        - color = argument.
 *        - VAO/VBO are generated and filled with a triangle fan that draws
 *          a filled circle.
 * @param {radius} radius of the ball in world units (used as Transform.scale).
 * @param {segments} how many edges approximate the circle (more = smoother).
 * @param {position} initial position of the ball center.
 * @param {color} the ball's uniform color (default = black transparent).
 * @param {g} gravity vector (default = {0, 9.80665}).
 *
 * Geometry setup:
 *  - Builds a triangle fan starting at (0,0), then radiates out
 *    vertices in a circle with angle_step = 2π / segments.
 *  - Scale (radius) is applied directly to each vertex.
 *  - The VAO/VBO are set up once and reused every frame.
 *
 * @returns Entity ID of the new ball.
 */
Entity createBall(std::unordered_map<Entity, ecs::comp::Transform> &transforms,
                  std::unordered_map<Entity, ecs::comp::Physics> &physics,
                  std::unordered_map<Entity, ecs::comp::Render> &renderables,
                  float radius, int segments, glm::vec2 position,
                  Color color = {0, 0, 0, 0}, glm::vec2 g = {0.0f, 9.80665f});
} // namespace ent
} // namespace ecs