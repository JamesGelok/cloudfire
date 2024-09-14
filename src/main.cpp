#include "./core/Entity.h"
#include "./core/EntityInitializer.h"
#include "./systems/InputSystem.h"
#include "./systems/MovementSystem.h"
#include "./systems/PhysicsSystem.h"
#include "./systems/RenderSystem.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float TARGET_FPS = 60.0f;
const float TARGET_FRAME_TIME = 1.0f / TARGET_FPS;

GLFWwindow *window;
EntityManager entityManager;
ComponentManager componentManager;

bool initOpenGL() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return false;
  }

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ECS Physics Box",
                            nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);

  return true;
}

void cleanup() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

int main() {
  if (!initOpenGL()) {
    return -1;
  }

  // Initialize entities
  initializeEntities(entityManager, componentManager);

  // Initialize systems
  InputSystem inputSystem;
  MovementSystem movementSystem(&inputSystem);
  PhysicsSystem physicsSystem;
  RenderSystem renderSystem;

  float lastTime = glfwGetTime();
  float accumulator = 0.0f;

  // Main game loop
  while (!glfwWindowShouldClose(window)) {
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    accumulator += deltaTime;

    glfwPollEvents();
    inputSystem.update(window);

    while (accumulator >= TARGET_FRAME_TIME) {
      movementSystem.update(TARGET_FRAME_TIME, entityManager, componentManager);
      physicsSystem.update(TARGET_FRAME_TIME, entityManager, componentManager);
      accumulator -= TARGET_FRAME_TIME;
    }

    renderSystem.update(deltaTime, entityManager, componentManager);
  }

  cleanup();
  return 0;
}
