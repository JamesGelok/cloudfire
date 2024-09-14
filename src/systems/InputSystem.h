#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

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

InputSystem::InputSystem() {
  keyStates[GLFW_KEY_UP] = false;
  keyStates[GLFW_KEY_DOWN] = false;
  keyStates[GLFW_KEY_LEFT] = false;
  keyStates[GLFW_KEY_RIGHT] = false;
}

void InputSystem::update(GLFWwindow *window) {
  keyStates[GLFW_KEY_UP] = (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);
  keyStates[GLFW_KEY_DOWN] = (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS);
  keyStates[GLFW_KEY_LEFT] = (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
  keyStates[GLFW_KEY_RIGHT] =
      (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);
}

bool InputSystem::isKeyPressed(int key) const {
  auto it = keyStates.find(key);
  return (it != keyStates.end()) ? it->second : false;
}

#endif // INPUT_SYSTEM_H