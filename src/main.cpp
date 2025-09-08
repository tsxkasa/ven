#include "ball.h"
#include "pch.h"
#include "shadercompile.h"
#include "ui_main.h"

int main() {
  GLFWwindow *window;
  if (!glfwInit()) {
    return -1;
  }

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

  // Set window hints to create a borderless window
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  glfwWindowHint(GLFW_RED_BITS, vmode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, vmode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, vmode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, vmode->refreshRate);

  window =
      glfwCreateWindow(vmode->width, vmode->height, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  glfwSetWindowPos(window, 0, 0);
  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");

  GLuint shaderProgram =
      CreateShaderProgram(readFile("shaders/ball.vert").c_str(),
                          readFile("shaders/ball.frag").c_str());

  glm::mat4 projection =
      glm::ortho(0.0f, (float)vmode->width, (float)vmode->height, 0.0f);
  glm::mat4 view = glm::mat4(1.0f);

  Ball ball(50.0f, 50, "Ball", glm::vec2(100.0f, 100.0f));

  // ---- Main draw loop ----
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (gui::render::exit_button()) {
      break;
    }

    ImGui::Render();
    ball.draw(shaderProgram, projection, view);

    glViewport(0, 0, vmode->width, vmode->height);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}