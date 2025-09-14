#include "ui_main.h"

bool gui::render::exit_button() {
  ImVec2 windowPos = ImVec2(1700.0f, 800.0f);
  ImVec2 windowSize = ImVec2(170.0f, 170.0f);
  ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
  ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
  ImGui::Begin("##quit", nullptr,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
                   ImGuiWindowFlags_NoScrollbar);

  bool exit = false;
  if (ImGui::Button("Quit", ImVec2(150.0f, 150.0f))) {
    exit = true;
  }
  ImGui::End();
  return exit;
}

void gui::render::framerate(double fps) {
  ImVec2 windowPos = ImVec2(1700.0f, 200.0f);
  ImVec2 windowSize = ImVec2(150.0f, 150.0f);
  ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
  ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
  ImGui::Begin("##frame", nullptr,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
                   ImGuiWindowFlags_NoScrollbar);

  ImGui::Text("FPS: %.2f", fps);
  ImGui::End();
}

void gui::render::cursor_pos(double x, double y) {
  ImVec2 windowPos = ImVec2(1700.0f, 400.0f);
  ImVec2 windowSize = ImVec2(300.0f, 300.0f);
  ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
  ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
  ImGui::Begin("##cursor", nullptr,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
                   ImGuiWindowFlags_NoScrollbar);

  ImGui::Text("Cursor: (%.2f, %.2f)", x, y);
  ImGui::End();
}

void gui::render::object_amt(int object_amt) {
  ImVec2 windowPos = ImVec2(1700.0f, 300.0f);
  ImVec2 windowSize = ImVec2(300.0f, 300.0f);
  ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
  ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
  ImGui::Begin("##objects", nullptr,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
                   ImGuiWindowFlags_NoScrollbar);

  ImGui::Text("Amount: %d", object_amt);
  ImGui::End();
}

void gui::render::clear_obj(ecs::management::ResourceControl &resources) {
  ImVec2 windowPos = ImVec2(200.0f, 800.0f);
  ImVec2 windowSize = ImVec2(170.0f, 170.0f);
  ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
  ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
  ImGui::Begin("##clear_scene", nullptr,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
                   ImGuiWindowFlags_NoScrollbar);
  if (ImGui::Button("Clear Scene", ImVec2(100.0f, 100.0f)))
    resources.clear_scene();
  ImGui::End();
}