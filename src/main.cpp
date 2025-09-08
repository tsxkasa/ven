#include "ball.h"
#include "pch.h"
#include "shadercompile.h"

int main() {
  GLFWwindow *window;
  if (!glfwInit()) {
    return -1;
  }
  /*
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
      std::cerr << "No monitor detected.";
      return -1;
    }

    const GLFWvidmode *vmode = glfwGetVideoMode(monitor);
    if (!vmode) {
      std::cerr << "Unexpected error occured.";
      return -1;
    }
  */

  window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  GLuint shaderProgram =
      CreateShaderProgram(readFile("shaders/ball.vert").c_str(),
                          readFile("shaders/ball.frag").c_str());

  glm::mat4 projection = glm::ortho(0.0f, (float)800.0f, (float)600.0f, 0.0f);
  glm::mat4 view = glm::mat4(1.0f);

  Ball ball(50.0f, 50, "Ball", glm::vec2(100.0f, 100.0f));

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ball.draw(shaderProgram, projection, view);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}