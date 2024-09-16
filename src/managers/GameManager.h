#pragma once

#include "../core/Entity.h"
#include "../core/EntityInitializer.h"
#include "./ComponentManager.h"

class GameManager {
public:
  GameManager(EntityManager &entityManager, ComponentManager &componentManager);

  void resetGame();

private:
  EntityManager &entityManager;
  ComponentManager &componentManager;
};
