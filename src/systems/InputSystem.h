#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream> // For logging
#include <unordered_map>

class InputSystem {
public:
  InputSystem();

  void update(GLFWwindow *window);

  bool isKeyPressed(int key) const;

private:
  std::unordered_map<int, bool> keyStates;
};

InputSystem::InputSystem() {
  // Initialize key states for movement in X, Y, and Z axes
  keyStates[GLFW_KEY_UP] = false;
  keyStates[GLFW_KEY_DOWN] = false;
  keyStates[GLFW_KEY_LEFT] = false;
  keyStates[GLFW_KEY_RIGHT] = false;
  keyStates[GLFW_KEY_W] = false; // Forward (Z-axis)
  keyStates[GLFW_KEY_S] = false; // Backward (Z-axis)
}

void InputSystem::update(GLFWwindow *window) {
  // Update key states for movement in X and Y axes
  keyStates[GLFW_KEY_UP] = (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);
  keyStates[GLFW_KEY_DOWN] = (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS);
  keyStates[GLFW_KEY_LEFT] = (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
  keyStates[GLFW_KEY_RIGHT] =
      (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);

  // Update key states for Z-axis movement (forward/backward)
  keyStates[GLFW_KEY_W] = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
  keyStates[GLFW_KEY_S] = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);

  // Log the state of relevant keys
  // std::cout << "Input States:\n";
  // std::cout << "UP: " << keyStates[GLFW_KEY_UP]
  //           << " | DOWN: " << keyStates[GLFW_KEY_DOWN] << "\n";
  // std::cout << "LEFT: " << keyStates[GLFW_KEY_LEFT]
  //           << " | RIGHT: " << keyStates[GLFW_KEY_RIGHT] << "\n";
  // std::cout << "FORWARD (W): " << keyStates[GLFW_KEY_W]
  //           << " | BACKWARD (S): " << keyStates[GLFW_KEY_S] << "\n";
  // std::cout << "----------------------\n";
}

bool InputSystem::isKeyPressed(int key) const {
  auto it = keyStates.find(key);
  return (it != keyStates.end()) ? it->second : false;
}

#endif // INPUT_SYSTEM_H
