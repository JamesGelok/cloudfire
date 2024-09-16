#include "EntityInitializer.h"
#include "../components/Acceleration.h"
#include "../components/Collidable.h"
#include "../components/GravityAffected.h"
#include "../components/Material.h"
#include "../components/ModelLoader.h"
#include "../components/PlayerControlled.h"
#include "../components/Position.h"
#include "../components/Renderable.h"
#include "../components/Rotation.h"
#include "../components/Scale.h"
#include "../components/Velocity.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

// Constants and helper functions scoped within this file
namespace {
const float MIN_PLATFORM_DISTANCE = 10.0f;
const float MAX_PLATFORM_DISTANCE = 20.0f;
const float PLATFORM_STEP_HEIGHT = 0.5f;
const int PLATFORM_COUNT = 10;

// Random number generator
std::mt19937 &getRandomEngine() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  return gen;
}

float randomFloat(float min, float max) {
  std::uniform_real_distribution<> dis(min, max);
  return dis(getRandomEngine());
}

glm::vec3 getNextPlatformPosition(const glm::vec3 &lastPos,
                                  const glm::vec3 &lastDir) {
  float angle = randomFloat(-glm::radians(45.0f), glm::radians(45.0f));

  // Create a rotation matrix to rotate the direction vector around Y-axis
  glm::mat4 rotationMatrix =
      glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));

  // Rotate the last direction vector to get the new direction
  glm::vec3 newDir = glm::vec3(rotationMatrix * glm::vec4(lastDir, 0.0f));

  // Calculate a random distance within the allowed range
  float distance = randomFloat(MIN_PLATFORM_DISTANCE, MAX_PLATFORM_DISTANCE);

  // Calculate the new position based on the rotated direction and distance
  glm::vec3 newPos = lastPos + (newDir * distance);

  // Add the step height to move the platform upward
  newPos.y += PLATFORM_STEP_HEIGHT;

  return newPos;
}

// Helper function to initialize a platform
void initializePlatform(EntityManager &entityManager,
                        ComponentManager &componentManager, float x, float y,
                        float z, float scaleX = 5.0f, float scaleY = 1.0f,
                        float scaleZ = 5.0f) {
  EntityID platform = entityManager.createEntity();

  componentManager.addComponent(platform, Position(x, y, z), entityManager);
  componentManager.addComponent(platform, Velocity(0.0f, 0.0f, 0.0f),
                                entityManager);
  componentManager.addComponent(platform, Acceleration(0.0f, 0.0f, 0.0f),
                                entityManager);

  // Load the 3D cube model for the platform
  Renderable3D renderable3D;
  ModelLoader::loadModel("assets/models/cube.obj", renderable3D);
  componentManager.addComponent(platform, renderable3D, entityManager);

  // Set the material color for the platform (grey)
  Material platformMaterial(glm::vec3(0.8f, 0.8f, 0.8f), 0.5f, 32.0f);
  componentManager.addComponent(platform, platformMaterial, entityManager);

  // Add collidable tag for the platform
  componentManager.addComponent(platform, Collidable(), entityManager);

  // Add scale component to set the platform size
  componentManager.addComponent(platform, Scale(scaleX, scaleY, scaleZ),
                                entityManager);
}

// Function to initialize the player
void initializePlayer(EntityManager &entityManager,
                      ComponentManager &componentManager) {
  EntityID player = entityManager.createEntity();

  componentManager.addComponent(player, Position(0.0f, 2.0f, 0.0f),
                                entityManager);
  // Start above the first platform
  componentManager.addComponent(player, Velocity(0.0f, 0.0f, 0.0f),
                                entityManager);
  componentManager.addComponent(player, Acceleration(0.0f, 0.0f, 0.0f),
                                entityManager);

  // Load the 3D cube model for the player
  Renderable3D renderable3D;
  ModelLoader::loadModel("assets/models/cube.obj", renderable3D);
  componentManager.addComponent(player, renderable3D, entityManager);

  // Set the material color for the player cube (orange)
  Material playerMaterial(glm::vec3(1.0f, 0.5f, 0.0f), 0.5f, 32.0f);
  componentManager.addComponent(player, playerMaterial, entityManager);

  // Add player-controlled tag
  componentManager.addComponent(player, PlayerControlled(), entityManager);

  // Add collidable tag
  componentManager.addComponent(player, Collidable(), entityManager);

  // Add gravity-affected tag
  componentManager.addComponent(player, GravityAffected(), entityManager);

  // Add rotation component
  componentManager.addComponent(player, Rotation(), entityManager);

  // Add scale component to adjust the player's size if necessary
  componentManager.addComponent(player, Scale(1.0f, 1.0f, 1.0f), entityManager);
}
} // unnamed namespace

// Definition of initializeEntities
void initializeEntities(EntityManager &entityManager,
                        ComponentManager &componentManager) {
  // Initialize the player-controlled red cube
  initializePlayer(entityManager, componentManager);

  // Initialize the starting platform
  glm::vec3 lastPlatformPos(0.0f, 0.0f, 0.0f); // Starting at the origin
  // Initial direction along the X-axis
  glm::vec3 lastDirection(1.0f, 0.0f, 0.0f);

  initializePlatform(entityManager, componentManager, lastPlatformPos.x,
                     lastPlatformPos.y, lastPlatformPos.z, 10.0f, 1.0f, 10.0f);

  // Generate and initialize a path of platforms
  for (int i = 1; i <= PLATFORM_COUNT; ++i) {
    glm::vec3 nextPlatformPos =
        getNextPlatformPosition(lastPlatformPos, lastDirection);
    initializePlatform(entityManager, componentManager, nextPlatformPos.x,
                       nextPlatformPos.y, nextPlatformPos.z);

    // Update the last platform position and direction for the next iteration
    lastDirection = glm::normalize(nextPlatformPos - lastPlatformPos);
    lastPlatformPos = nextPlatformPos;
  }
}
