#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <vector>

// 2D Renderable structure for existing 2D systems
struct Renderable2D {
  float width;
  float height;
  glm::vec3 color;

  // Vertex data
  std::vector<glm::vec2> vertices;
  GLuint VAO, VBO;

  Renderable2D(float _width = 1.0f, float _height = 1.0f, float r = 1.0f,
               float g = 1.0f, float b = 1.0f)
      : width(_width), height(_height), color(r, g, b), VAO(0), VBO(0) {}
};

// 3D Renderable structure for 3D models
struct Renderable3D {
  // Store vertex data
  std::vector<glm::vec3> vertices;

  // Store normal data for lighting
  std::vector<glm::vec3> normals;

  // Store indices for indexed drawing
  std::vector<GLuint> indices;

  // OpenGL buffers
  GLuint VAO, VBO, EBO;

  // Constructor for 3D models
  Renderable3D() : VAO(0), VBO(0), EBO(0) {}
};

#endif // RENDERABLE_H
