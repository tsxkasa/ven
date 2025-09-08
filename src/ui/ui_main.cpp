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

  if (ImGui::Button("Quit", ImVec2(150.0f, 150.0f))) {
    ImGui::End();
    return true;
  }
  ImGui::End();
  return false;
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