#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct Rotation {
  glm::quat quaternion;
  glm::vec3 angularVelocity;
  glm::vec3 angularAcceleration;

  Rotation();
  Rotation(const glm::quat &initialQuaternion);
  Rotation(const glm::quat &initialQuaternion,
           const glm::vec3 &initialAngularVelocity);
  Rotation(const glm::quat &initialQuaternion,
           const glm::vec3 &initialAngularVelocity,
           const glm::vec3 &initialAngularAcceleration);
};
