#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

// Material component to store lighting and color properties
struct Material {
  glm::vec3 diffuseColor; // Base color of the material
  float specularStrength; // How reflective the surface is
  float shininess;        // Determines the size of the specular highlight

  // Default constructor with gray diffuse color and basic specular properties
  Material(const glm::vec3 &color = glm::vec3(0.5f, 0.5f, 0.5f),
           float specular = 0.5f, float shiny = 32.0f)
      : diffuseColor(color), specularStrength(specular), shininess(shiny) {}
};

#endif // MATERIAL_H
