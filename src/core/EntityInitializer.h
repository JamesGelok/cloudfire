#ifndef ENTITY_INITIALIZER_H
#define ENTITY_INITIALIZER_H

#include "../core/Entity.h"
#include "../managers/ComponentManager.h"

void initializeEntities(EntityManager &entityManager,
                        ComponentManager &componentManager) {
  // Create a box entity and add components using the component manager
  EntityID box = entityManager.createEntity();
  componentManager.addComponent(box, Position(10, 10), entityManager);
  componentManager.addComponent(box, Velocity(0, 0), entityManager);
  componentManager.addComponent(box, Acceleration(0.0f, 0.0f), entityManager);
  componentManager.addComponent(box, Renderable(50, 50, 1.0f, 0.0f, 0.0f),
                                entityManager);

  EntityID box2 = entityManager.createEntity();
  componentManager.addComponent(box2, Position(100, 100), entityManager);
  componentManager.addComponent(box2, Velocity(0, 0), entityManager);
  componentManager.addComponent(box2, Acceleration(0.0f, 0.0f), entityManager);
  componentManager.addComponent(box2, Renderable(50, 50, 0.0f, 1.0f, 0.0f),
                                entityManager);
}

#endif // ENTITY_INITIALIZER_H
