#pragma once

#include <glm/glm.hpp>

struct Scale {
  glm::vec3 scale;

  Scale(float x = 1.0f, float y = 1.0f, float z = 1.0f);
};
