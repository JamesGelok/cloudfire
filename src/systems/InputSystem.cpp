#include "InputSystem.h"

InputSystem::InputSystem() {
  keyStates[GLFW_KEY_UP] = false;
  keyStates[GLFW_KEY_DOWN] = false;
  keyStates[GLFW_KEY_LEFT] = false;
  keyStates[GLFW_KEY_RIGHT] = false;
  keyStates[GLFW_KEY_SPACE] = false;
  keyStates[GLFW_KEY_W] = false;
  keyStates[GLFW_KEY_S] = false;
  keyStates[GLFW_KEY_A] = false;
  keyStates[GLFW_KEY_D] = false;
}

void InputSystem::update(GLFWwindow *window) {
  // Update key states
  keyStates[GLFW_KEY_UP] = (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);
  keyStates[GLFW_KEY_DOWN] = (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS);
  keyStates[GLFW_KEY_LEFT] = (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
  keyStates[GLFW_KEY_RIGHT] =
      (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);
  keyStates[GLFW_KEY_SPACE] =
      (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
  keyStates[GLFW_KEY_W] = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
  keyStates[GLFW_KEY_S] = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
  keyStates[GLFW_KEY_A] = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
  keyStates[GLFW_KEY_D] = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
}

bool InputSystem::isKeyPressed(int key) const {
  auto it = keyStates.find(key);
  return (it != keyStates.end()) ? it->second : false;
}
