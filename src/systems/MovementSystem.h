#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../components/Acceleration.h"
#include "../components/Position.h"
#include "../components/Velocity.h"
#include "../core/Entity.h"
#include "../managers/ComponentManager.h"
#include "../systems/InputSystem.h"
#include <algorithm>
#include <iostream>

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

  for (EntityID entity = 0; entity < entityManager.entityCount(); ++entity) {

    const ComponentMask &mask = entityManager.getComponentMask(entity);

    if (mask.test(ComponentType<Acceleration>::ID()) &&
        mask.test(ComponentType<Position>::ID())) {

      auto *acceleration = componentManager.getComponent<Acceleration>(entity);
      if (acceleration) {

        if (inputSystem->isKeyPressed(GLFW_KEY_UP)) {
          acceleration->ay += 150.0f * deltaTime;
        } else if (inputSystem->isKeyPressed(GLFW_KEY_DOWN)) {
          acceleration->ay -= 150.0f * deltaTime;
        } else {
          acceleration->ay = 0.0f;
        }

        if (inputSystem->isKeyPressed(GLFW_KEY_LEFT)) {
          acceleration->ax -= 150.0f * deltaTime;
        } else if (inputSystem->isKeyPressed(GLFW_KEY_RIGHT)) {
          acceleration->ax += 150.0f * deltaTime;
        } else {
          acceleration->ax = 0.0f;
        }

        // Update acceleration along the Z-axis
        if (inputSystem->isKeyPressed(GLFW_KEY_W)) {
          acceleration->az += 150.0f * deltaTime;
        } else if (inputSystem->isKeyPressed(GLFW_KEY_S)) {
          acceleration->az -= 150.0f * deltaTime;
        } else {
          acceleration->az = 0.0f;
        }

        std::cout << "Entity: " << entity
                  << " | Acceleration X: " << acceleration->ax
                  << " | Acceleration Y: " << acceleration->ay
                  << " | Acceleration Z: " << acceleration->az << std::endl;

        acceleration->ax =
            std::clamp(acceleration->ax, -MAX_ACCELERATION, MAX_ACCELERATION);
        acceleration->ay =
            std::clamp(acceleration->ay, -MAX_ACCELERATION, MAX_ACCELERATION);
        acceleration->az =
            std::clamp(acceleration->az, -MAX_ACCELERATION, MAX_ACCELERATION);
      }
    }
  }
}

#endif
