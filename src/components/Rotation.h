#ifndef ROTATION_H
#define ROTATION_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct Rotation {
  glm::quat quaternion;      // Represents the current rotation as a quaternion
  glm::vec3 angularVelocity; // Angular velocity (rotation speed around X, Y, Z)
  glm::vec3 angularAcceleration; // Angular acceleration (rate of change of
                                 // angular velocity)

  // Default constructor initializes identity quaternion (no rotation)
  Rotation()
      : quaternion(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
        angularVelocity(0.0f, 0.0f, 0.0f),
        angularAcceleration(0.0f, 0.0f, 0.0f) {}

  // Constructor to initialize with a quaternion directly
  Rotation(const glm::quat &initialQuaternion)
      : quaternion(initialQuaternion), angularVelocity(0.0f, 0.0f, 0.0f),
        angularAcceleration(0.0f, 0.0f, 0.0f) {}

  // Constructor to initialize with a quaternion and angular velocity
  Rotation(const glm::quat &initialQuaternion,
           const glm::vec3 &initialAngularVelocity)
      : quaternion(initialQuaternion), angularVelocity(initialAngularVelocity),
        angularAcceleration(0.0f, 0.0f, 0.0f) {}

  // Constructor to initialize with a quaternion, angular velocity, and angular
  // acceleration
  Rotation(const glm::quat &initialQuaternion,
           const glm::vec3 &initialAngularVelocity,
           const glm::vec3 &initialAngularAcceleration)
      : quaternion(initialQuaternion), angularVelocity(initialAngularVelocity),
        angularAcceleration(initialAngularAcceleration) {}
};

#endif // ROTATION_H
