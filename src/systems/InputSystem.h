#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <unordered_map>

class InputSystem {
public:
  InputSystem();

  void update(GLFWwindow *window);

  bool isKeyPressed(int key) const;

private:
  std::unordered_map<int, bool> keyStates;
};
