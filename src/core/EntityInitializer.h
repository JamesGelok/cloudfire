// src/core/EntityInitializer.h
#ifndef ENTITY_INITIALIZER_H
#define ENTITY_INITIALIZER_H

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
#include "../core/Entity.h"
#include "../managers/ComponentManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

void initializePlayer(EntityManager &entityManager,
                      ComponentManager &componentManager) {
  EntityID player = entityManager.createEntity();

  componentManager.addComponent(player, Position(0.0f, 1.0f, 0.0f),
                                entityManager); // Start above the ground
  componentManager.addComponent(player, Velocity(0.0f, 0.0f, 0.0f),
                                entityManager);
  componentManager.addComponent(player, Acceleration(0.0f, 0.0f, 0.0f),
                                entityManager);

  // Load the 3D cube model for the player
  Renderable3D renderable3D;
  ModelLoader::loadModel("assets/models/cube.obj", renderable3D);
  componentManager.addComponent(player, renderable3D, entityManager);

  // Set the material color for the player cube (red)
  Material playerMaterial(glm::vec3(1.0f, 0.0f, 0.0f), 0.5f, 32.0f);
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

// Helper function to initialize ground/platform entity
void initializeGround(EntityManager &entityManager,
                      ComponentManager &componentManager) {
  EntityID ground = entityManager.createEntity();

  componentManager.addComponent(ground, Position(0.0f, 0.0f, 0.0f),
                                entityManager); // Ground at y = 0
  componentManager.addComponent(ground, Velocity(0.0f, 0.0f, 0.0f),
                                entityManager);
  componentManager.addComponent(ground, Acceleration(0.0f, 0.0f, 0.0f),
                                entityManager);

  // Load the 3D cube model for the ground
  Renderable3D renderable3D;
  ModelLoader::loadModel("assets/models/cube.obj", renderable3D);
  componentManager.addComponent(ground, renderable3D, entityManager);

  // Set the material color for the ground (grey)
  Material groundMaterial(glm::vec3(0.5f, 0.5f, 0.5f), 0.5f, 32.0f);
  componentManager.addComponent(ground, groundMaterial, entityManager);

  // Add collidable tag for ground
  componentManager.addComponent(ground, Collidable(), entityManager);

  // Add scale component to make the ground larger
  componentManager.addComponent(ground, Scale(50.0f, 1.0f, 50.0f),
                                entityManager);
}

// Main function to initialize all entities
void initializeEntities(EntityManager &entityManager,
                        ComponentManager &componentManager) {
  // Initialize the player-controlled red cube
  initializePlayer(entityManager, componentManager);

  // Initialize ground
  initializeGround(entityManager, componentManager);
}

#endif // ENTITY_INITIALIZER_H
