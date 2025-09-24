#include "keyboard_movement_controller_system.h"
#include "transform.h"

void ecs::sys::KeyboardMovementControllerSystem::moveInPlaneXZ(
    GLFWwindow* window, float dt) {
  for (const auto& ent : m_entities) {
    auto& transform3d = gCoordinator->getComponent<ecs::comp::Transform3D>(ent);

    glm::vec3 rotate{0};
    if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS)
      rotate.y += 1.0f;
    if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS)
      rotate.y -= 1.0f;
    if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS)
      rotate.x += 1.0f;
    if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS)
      rotate.x -= 1.0f;

    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
      transform3d.rotation += lookSpeed * dt * glm::normalize(rotate);
    }

    // FIX #1: Use a valid range for the clamp to allow vertical rotation.
    transform3d.rotation.x = glm::clamp(transform3d.rotation.x, -1.5f, 1.5f);
    transform3d.rotation.y =
        glm::mod(transform3d.rotation.y, glm::two_pi<float>());

    float yaw = transform3d.rotation.y;
    const glm::vec3 forwardDir{sin(yaw), 0.0f, cos(yaw)};
    // FIX #2: Correctly calculate the right-direction vector.
    const glm::vec3 rightDir{forwardDir.z, 0.0f, -forwardDir.x};
    const glm::vec3 upDir{0.0f, -1.0f, 0.0f};

    glm::vec3 moveDir{0.0f};
    if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)
      moveDir += forwardDir;
    if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)
      moveDir -= forwardDir;
    if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)
      moveDir -= rightDir;
    if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)
      moveDir += rightDir;
    if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS)
      moveDir -= upDir;
    if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS)
      moveDir += upDir;

    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
      transform3d.translation += moveSpeed * dt * glm::normalize(moveDir);
    }
  }
}