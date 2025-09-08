#include "ui_main.h"

bool gui::render::exit_button() {
  ImGui::Begin("##quit");

  if (ImGui::Button("Quit")) {
    ImGui::End();
    return true;
  }
  ImGui::End();
  return false;
}