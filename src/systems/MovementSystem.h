#pragma once

#include "../components/Acceleration.h"
#include "../components/OnGround.h"
#include "../components/PlayerControlled.h"
#include "../components/Position.h"
#include "../components/Rotation.h"
#include "../components/Velocity.h"
#include "../core/Entity.h"
#include "../managers/ComponentManager.h"
#include "../systems/InputSystem.h"

class MovementSystem {
private:
  InputSystem *inputSystem;

public:
  MovementSystem(InputSystem *inputSys);

  void update(float deltaTime, EntityManager &entityManager,
              ComponentManager &componentManager);
};
