#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
  glm::vec3 position; // Change to vec3 for 3D vertices
  glm::vec3 color;
};

struct Renderable {
  float width;
  float height;
  glm::vec3 color;

  // Vertex data
  std::vector<Vertex> vertices;
  GLuint VAO, VBO;

  Renderable(float _width = 1.0f, float _height = 1.0f, float r = 1.0f,
             float g = 1.0f, float b = 1.0f)
      : width(_width), height(_height), color(r, g, b), VAO(0), VBO(0) {}
};

#endif // RENDERABLE_H
