#ifndef ENTITY_INITIALIZER_H
#define ENTITY_INITIALIZER_H

#include "../components/Acceleration.h"
#include "../components/Material.h"
#include "../components/ModelLoader.h"
#include "../components/PlayerControlled.h" // Include PlayerControlled component
#include "../components/Position.h"
#include "../components/Renderable.h"
#include "../components/Rotation.h"
#include "../components/Velocity.h"
#include "../core/Entity.h"
#include "../managers/ComponentManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// Helper function to initialize the player-controlled red cube
void initializePlayer(EntityManager &entityManager,
                      ComponentManager &componentManager) {
  EntityID player = entityManager.createEntity();

  // Add position, velocity, and other components to the player entity
  componentManager.addComponent(player, Position(0.0f, 0.0f, 0.0f),
                                entityManager);
  componentManager.addComponent(player, Velocity(0.0f, 0.0f, 0.0f),
                                entityManager);
  componentManager.addComponent(player, Acceleration(0.0f, 0.0f, 0.0f),
                                entityManager);

  // Set the red color for the player cube
  componentManager.addComponent(player, Renderable2D(50, 50, 1.0f, 0.0f, 0.0f),
                                entityManager); // Red color

  // Add player-controlled tag
  componentManager.addComponent(player, PlayerControlled(), entityManager);

  // Add rotation (optional, if the player cube rotates)
  componentManager.addComponent(player, Rotation(), entityManager);
}

// Helper function to initialize white rotating cubes
void initializeRotatingCube(EntityManager &entityManager,
                            ComponentManager &componentManager, float x,
                            float y) {
  EntityID cube = entityManager.createEntity();

  // Add position, velocity, and rotation components to the cube entity
  componentManager.addComponent(cube, Position(x, y, 0.0f), entityManager);
  componentManager.addComponent(cube, Velocity(0.0f, 0.0f, 0.0f),
                                entityManager);
  componentManager.addComponent(cube, Acceleration(0.0f, 0.0f, 0.0f),
                                entityManager);

  // Set the white color for the cube
  componentManager.addComponent(cube, Renderable2D(50, 50, 1.0f, 1.0f, 1.0f),
                                entityManager);

  // Apply rotation around the Z-axis
  glm::quat rotation = glm::angleAxis(
      glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Initial rotation
  glm::vec3 angularVelocity(0.0f, 0.0f, 1.0f); // Rotate around the Z-axis
  componentManager.addComponent(cube, Rotation(rotation, angularVelocity),
                                entityManager);
}

// Main function to initialize all entities
void initializeEntities(EntityManager &entityManager,
                        ComponentManager &componentManager) {
  // Initialize the player-controlled red cube
  initializePlayer(entityManager, componentManager);

  // Initialize rotating white cubes
  initializeRotatingCube(entityManager, componentManager, 100.0f,
                         100.0f); // Example position (100, 100)
  initializeRotatingCube(entityManager, componentManager, -100.0f,
                         200.0f); // Another example position (-100, 200)
}

#endif
