#pragma once

#include "glad/glad.h"
#include <string>

class Shader {
public:
  GLuint ID;

  Shader(const char *vertexPath, const char *fragmentPath);

  void use();
  void setMat4(const std::string &name, const GLfloat *value);
  void setVec3(const std::string &name, float x, float y, float z);
  void setFloat(const std::string &name, float value);
};
