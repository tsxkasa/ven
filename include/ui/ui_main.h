#pragma once

#include "pch.h"
#include "ecs.h"

namespace gui {
namespace render {
bool exit_button();
void framerate(double fps);
void cursor_pos(double x, double y);
void object_amt(int object_num);
void clear_obj(ecs::management::ResourceControl &resources);
}; // namespace render
}; // namespace gui