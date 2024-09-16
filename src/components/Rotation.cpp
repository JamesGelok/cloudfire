#include "./Rotation.h"

Rotation::Rotation()
    : quaternion(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
      angularVelocity(0.0f, 0.0f, 0.0f), angularAcceleration(0.0f, 0.0f, 0.0f) {
}

Rotation::Rotation(const glm::quat &initialQuaternion)
    : quaternion(initialQuaternion), angularVelocity(0.0f, 0.0f, 0.0f),
      angularAcceleration(0.0f, 0.0f, 0.0f) {}

Rotation::Rotation(const glm::quat &initialQuaternion,
                   const glm::vec3 &initialAngularVelocity)
    : quaternion(initialQuaternion), angularVelocity(initialAngularVelocity),
      angularAcceleration(0.0f, 0.0f, 0.0f) {}

Rotation::Rotation(const glm::quat &initialQuaternion,
                   const glm::vec3 &initialAngularVelocity,
                   const glm::vec3 &initialAngularAcceleration)
    : quaternion(initialQuaternion), angularVelocity(initialAngularVelocity),
      angularAcceleration(initialAngularAcceleration) {}
