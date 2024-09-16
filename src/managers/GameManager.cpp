#include "./GameManager.h"

GameManager::GameManager(EntityManager &em, ComponentManager &cm)
    : entityManager(em), componentManager(cm) {
  // Initialize the game state
  initializeEntities(entityManager, componentManager);
}

void GameManager::resetGame() {
  // Clear all entities and components
  entityManager = EntityManager();
  componentManager = ComponentManager();

  // Re-initialize the game entities
  initializeEntities(entityManager, componentManager);
}
