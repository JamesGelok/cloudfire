#pragma once

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <vector>

struct Renderable2D {
  float width;
  float height;
  glm::vec3 color;

  std::vector<glm::vec2> vertices;
  GLuint VAO, VBO;

  Renderable2D(float _width = 1.0f, float _height = 1.0f, float r = 1.0f,
               float g = 1.0f, float b = 1.0f);
};

struct Renderable3D {
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;
  std::vector<GLuint> indices;

  GLuint VAO, VBO, EBO;

  Renderable3D();
};
