#include "ven_app.h"

void ven::App::run() {
  init();
  loop();
  cleanup();
}