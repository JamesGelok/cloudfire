#ifndef SYSTEM_H
#define SYSTEM_H

#include "Entity.h"
#include <vector>

class System {
public:
  virtual void update(float deltaTime, std::vector<Entity> &entities) = 0;
};

#endif // SYSTEM_H
