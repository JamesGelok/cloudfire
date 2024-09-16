#include "Material.h"

Material::Material(const glm::vec3 &color, float specular, float shiny)
    : diffuseColor(color), specularStrength(specular), shininess(shiny) {}
