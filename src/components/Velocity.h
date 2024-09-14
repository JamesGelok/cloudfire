#ifndef VELOCITY_H
#define VELOCITY_H

struct Velocity {
  float dx;
  float dy;
  Velocity(float _dx = 0.0f, float _dy = 0.0f) : dx(_dx), dy(_dy) {}
};

#endif // VELOCITY_H
