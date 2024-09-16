#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "./core/Entity.h"
#include "./core/EntityInitializer.h"
#include "./managers/ComponentManager.h"

class GameManager {
public:
  GameManager(EntityManager &entityManager, ComponentManager &componentManager);

  void resetGame();

private:
  EntityManager &entityManager;
  ComponentManager &componentManager;
};

GameManager::GameManager(EntityManager &em, ComponentManager &cm)
    : entityManager(em), componentManager(cm) {
  // Initialize the game state
  initializeEntities(entityManager, componentManager);
}

void GameManager::resetGame() {
  // Clear all entities and components
  entityManager = EntityManager();       // Reset entity manager
  componentManager = ComponentManager(); // Reset component manager

  // Re-initialize the game entities
  initializeEntities(entityManager, componentManager);
}

#endif // GAME_MANAGER_H
