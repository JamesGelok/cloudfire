#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../components/Acceleration.h"
#include "../components/Position.h"
#include "../components/Velocity.h"
#include "../core/Entity.h"
#include "../managers/ComponentManager.h"
#include "../systems/InputSystem.h"
#include <algorithm>

const float MAX_ACCELERATION = 200.0f;

class MovementSystem {
private:
  InputSystem *inputSystem;

public:
  MovementSystem(InputSystem *inputSys);

  void update(float deltaTime, EntityManager &entityManager,
              ComponentManager &componentManager);
};

MovementSystem::MovementSystem(InputSystem *inputSys) : inputSystem(inputSys) {}

void MovementSystem::update(float deltaTime, EntityManager &entityManager,
                            ComponentManager &componentManager) {
  // Loop over all entities
  for (EntityID entity = 0; entity < entityManager.entityCount(); ++entity) {

    const ComponentMask &mask = entityManager.getComponentMask(entity);

    // Check if the entity has both Acceleration and Position components
    if (mask.test(ComponentType<Acceleration>::ID()) && // Corrected ID() call
        mask.test(ComponentType<Position>::ID())) {     // Corrected ID() call
      auto *acceleration = componentManager.getComponent<Acceleration>(entity);
      if (acceleration) {
        // Update acceleration based on key input
        if (inputSystem->isKeyPressed(GLFW_KEY_UP)) {
          acceleration->ay += 50.0f * deltaTime;
        } else if (inputSystem->isKeyPressed(GLFW_KEY_DOWN)) {
          acceleration->ay -= 50.0f * deltaTime;
        } else {
          acceleration->ay = 0.0f; // Reset if no input
        }

        if (inputSystem->isKeyPressed(GLFW_KEY_LEFT)) {
          acceleration->ax -= 50.0f * deltaTime;
        } else if (inputSystem->isKeyPressed(GLFW_KEY_RIGHT)) {
          acceleration->ax += 50.0f * deltaTime;
        } else {
          acceleration->ax = 0.0f; // Reset if no input
        }

        // Clamp acceleration values
        acceleration->ax =
            std::clamp(acceleration->ax, -MAX_ACCELERATION, MAX_ACCELERATION);
        acceleration->ay =
            std::clamp(acceleration->ay, -MAX_ACCELERATION, MAX_ACCELERATION);
      }
    }
  }
}

#endif // MOVEMENT_SYSTEM_H
