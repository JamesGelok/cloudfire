#pragma once

#include "../components/Acceleration.h"
#include "../components/Collidable.h"
#include "../components/GravityAffected.h"
#include "../components/OnGround.h"
#include "../components/PlayerControlled.h"
#include "../components/Position.h"
#include "../components/Rotation.h"
#include "../components/Scale.h"
#include "../components/Velocity.h"
#include "../core/Entity.h"
#include "../managers/ComponentManager.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class PhysicsSystem {
public:
  void update(float deltaTime, EntityManager &entityManager,
              ComponentManager &componentManager);
};
