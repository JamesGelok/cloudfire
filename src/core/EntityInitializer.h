#ifndef ENTITY_INITIALIZER_H
#define ENTITY_INITIALIZER_H

#include "../components/Acceleration.h"
#include "../components/Material.h"
#include "../components/ModelLoader.h"
#include "../components/Position.h"
#include "../components/Renderable.h"
#include "../components/Rotation.h"
#include "../components/Velocity.h"
#include "../core/Entity.h"
#include "../managers/ComponentManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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
  componentManager.addComponent(model, Position(0.0f, 180.0f, -20.0f),
                                entityManager);
  componentManager.addComponent(model, Velocity(0, 0, 0), entityManager);
  componentManager.addComponent(model, Acceleration(0.0f, 0.0f, 0.0f),
                                entityManager);

  Renderable3D renderable3D;
  ModelLoader::loadModel("assets/models/cube.obj", renderable3D);
  componentManager.addComponent(model, renderable3D, entityManager);

  // Apply an isometric rotation
  glm::quat isometricRotation = glm::quat(glm::vec3(
      glm::radians(45.0f), glm::radians(35.26f), glm::radians(35.26f)));
  componentManager.addComponent(
      model, Rotation(isometricRotation, glm::vec3(1, 1, 1)), entityManager);

  // Add a Material component (light gray diffuse color, moderate shininess)
  Material modelMaterial(glm::vec3(0.8f, 0.8f, 0.8f), 0.5f, 32.0f);
  componentManager.addComponent(model, modelMaterial, entityManager);
  // Create a 3D model entity
  EntityID model2 = entityManager.createEntity();
  componentManager.addComponent(model2, Position(20.0f, 20.0f, -20.0f),
                                entityManager);
  componentManager.addComponent(model2, Velocity(0, 0, 0), entityManager);
  componentManager.addComponent(model2, Acceleration(0.0f, 0.0f, 0.0f),
                                entityManager);

  Renderable3D renderable3D2;
  ModelLoader::loadModel("assets/models/cube.obj", renderable3D2);
  componentManager.addComponent(model2, renderable3D2, entityManager);

  componentManager.addComponent(
      model2, Rotation(isometricRotation, glm::vec3(1, 1, 1)), entityManager);

  // Add a Material component (light gray diffuse color, moderate shininess)
  Material modelMaterial2(glm::vec3(0.8f, 0.8f, 0.8f), 0.5f, 32.0f);
  componentManager.addComponent(model2, modelMaterial2, entityManager);
}

#endif
