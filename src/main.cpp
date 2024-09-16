#include "./WindowConstants.h"
#include "./core/Entity.h"
#include "./managers/GameManager.h"
#include "./systems/InputSystem.h"
#include "./systems/MovementSystem.h"
#include "./systems/PhysicsSystem.h"
#include "./systems/RenderSystem.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

const float TARGET_FPS = 120.0f;
const float TARGET_FRAME_TIME = 1.0f / TARGET_FPS;
const float RESET_THRESHOLD = -50.0f;

GLFWwindow *window;
EntityManager entityManager;
ComponentManager componentManager;
GameManager *gameManager;

// Callback function to adjust the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

bool initOpenGL() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return false;
  }

  // Request OpenGL 3.3 Core Profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // If on macOS, uncomment the following line
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Enable double buffering explicitly
  glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE); // Ensure double buffering

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CloudFire", nullptr,
                            nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);

  // Initialize GLAD before calling any OpenGL functions
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return false;
  }

  // Print OpenGL version (optional for debugging)
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

  // Register callback for resizing the window
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  return true;
}

void cleanup() {
  delete gameManager;
  glfwDestroyWindow(window);
  glfwTerminate();
}

int main() {
  if (!initOpenGL()) {
    return -1;
  }

  // Enable VSync (set to 1 for enabling VSync)
  glfwSwapInterval(1);

  // Initialize game manager
  gameManager = new GameManager(entityManager, componentManager);

  // Initialize systems
  InputSystem inputSystem;
  MovementSystem movementSystem(&inputSystem);
  PhysicsSystem physicsSystem;
  RenderSystem renderSystem(window);

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

    // Game logic update
    while (accumulator >= TARGET_FRAME_TIME) {
      movementSystem.update(TARGET_FRAME_TIME, entityManager, componentManager);
      physicsSystem.update(TARGET_FRAME_TIME, entityManager, componentManager);
      accumulator -= TARGET_FRAME_TIME;
    }

    // Check if the player has fallen below the reset threshold
    // Find the player entity
    Position *playerPosition = nullptr;
    for (EntityID entity = 0; entity < entityManager.entityCount(); ++entity) {
      const ComponentMask &mask = entityManager.getComponentMask(entity);
      if (mask.test(ComponentType<PlayerControlled>::ID()) &&
          mask.test(ComponentType<Position>::ID())) {
        playerPosition = componentManager.getComponent<Position>(entity);
        break;
      }
    }

    // Render the scene
    renderSystem.update(deltaTime, entityManager, componentManager);

    // Swap the buffers (show the rendered frame)
    glfwSwapBuffers(window);

    if (playerPosition && playerPosition->y < RESET_THRESHOLD) {
      gameManager->resetGame();
      std::cout << "Player fell below threshold. Game reset." << std::endl;
      inputSystem = InputSystem();
      movementSystem = MovementSystem(&inputSystem);
      physicsSystem = PhysicsSystem();
      // render system needs to call reset method because it has OpenGL
      // resources that need to be cleaned up. In hindsight, I could've used
      // operator overloading to make this more readable
      renderSystem.reset();
      // Reset timing variables
      lastTime = glfwGetTime();
      accumulator = 0.0f;
    }
  }

  cleanup();
  return 0;
}
