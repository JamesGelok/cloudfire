#pragma once

#include "../core/Entity.h"
#include <ComponentManager.h>
#include <vector>

class System {
public:
  virtual void update(float deltaTime, EntityManager &entityManager,
                      ComponentManager &componentManager) = 0;
  virtual ~System() = default;
};
