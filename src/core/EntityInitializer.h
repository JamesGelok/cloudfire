#ifndef ENTITY_INITIALIZER_H
#define ENTITY_INITIALIZER_H

#include "../components/Acceleration.h"
#include "../components/ModelLoader.h" // Include the model loader
#include "../components/Position.h"
#include "../components/Renderable.h"
#include "../components/Rotation.h"
#include "../components/Velocity.h"
#include "../core/Entity.h"
#include "../managers/ComponentManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

void initializeEntities(EntityManager &entityManager,
                        ComponentManager &componentManager) {
  // Create a 2D box entity
  EntityID box = entityManager.createEntity();
  componentManager.addComponent(box, Position(180, 180, 0), entityManager);
  componentManager.addComponent(box, Velocity(0, 0, 0), entityManager);
  componentManager.addComponent(box, Acceleration(0.0f, 0.0f, 0.0f),
                                entityManager);
  componentManager.addComponent(box, Renderable2D(50, 50, 1.0f, 0.0f, 0.0f),
                                entityManager);
  componentManager.addComponent(box, Rotation(), entityManager);

  // Create a 3D model entity
  EntityID model = entityManager.createEntity();
  componentManager.addComponent(model, Position(20.0f, 20.0f, -20.0f),
                                entityManager);

  componentManager.addComponent(model, Velocity(0, 0, 0), entityManager);
  componentManager.addComponent(model, Acceleration(0.0f, 0.0f, 0.0f),
                                entityManager);

  // Load a 3D model using Assimp (ensure you have a valid model file path)
  Renderable3D renderable3D;
  ModelLoader::loadModel("assets/models/cube.obj",
                         renderable3D); // Specify the correct path

  componentManager.addComponent(model, renderable3D, entityManager);
  componentManager.addComponent(model, Rotation(), entityManager);
}

#endif // ENTITY_INITIALIZER_H
