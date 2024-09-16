#pragma once

#include <glm/glm.hpp>

struct Material {
  glm::vec3 diffuseColor;
  float specularStrength;
  float shininess;

  Material(const glm::vec3 &color = glm::vec3(0.5f, 0.5f, 0.5f),
           float specular = 0.5f, float shiny = 32.0f);
};
