#ifndef ENTITY_INITIALIZER_H
#define ENTITY_INITIALIZER_H

#include "../components/Acceleration.h"
#include "../components/Position.h"
#include "../components/Renderable.h"
#include "../components/Rotation.h" // Include rotation component
#include "../components/Velocity.h"
#include "../core/Entity.h"
#include "../managers/ComponentManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

void initializeEntities(EntityManager &entityManager,
                        ComponentManager &componentManager) {
  // Create a box entity and add components using the component manager
  EntityID box = entityManager.createEntity();
  componentManager.addComponent(box, Position(10, 10, 0), entityManager);
  componentManager.addComponent(box, Velocity(0, 0, 0), entityManager);
  componentManager.addComponent(box, Acceleration(0.0f, 0.0f, 0.0f),
                                entityManager);
  componentManager.addComponent(box, Renderable(50, 50, 1.0f, 0.0f, 0.0f),
                                entityManager);
  // Initialize with default quaternion (no rotation), zero angular velocity,
  // and acceleration
  componentManager.addComponent(box, Rotation(), entityManager);

  // Create a second box entity with a different rotation
  EntityID box2 = entityManager.createEntity();
  componentManager.addComponent(box2, Position(100, 100, 0), entityManager);
  componentManager.addComponent(box2, Velocity(0, 0, 0), entityManager);
  componentManager.addComponent(box2, Acceleration(0.0f, 0.0f, 0.0f),
                                entityManager);
  componentManager.addComponent(box2, Renderable(50, 50, 0.0f, 1.0f, 0.0f),
                                entityManager);
  // Initialize with a rotation of 45 degrees along the Y-axis and an angular
  // velocity
  componentManager.addComponent(
      box2,
      Rotation(glm::quat(glm::vec3(0.0f, glm::radians(45.0f), 0.0f)),
               glm::vec3(0.0f, 1.0f, 0.0f)),
      entityManager);
}

#endif // ENTITY_INITIALIZER_H
