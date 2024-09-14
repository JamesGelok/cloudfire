// src/Shader.h
#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include <string>

class Shader {
public:
  GLuint ID;

  Shader(const char *vertexPath, const char *fragmentPath);

  void use();
  void setMat4(const std::string &name, const GLfloat *value);

  // Additional utility functions as needed
};

#endif // SHADER_H
