#ifndef VELOCITY_H
#define VELOCITY_H

struct Velocity {
  float dx;
  float dy;
  float dz; // New field for Z velocity

  Velocity(float _dx = 0.0f, float _dy = 0.0f, float _dz = 0.0f)
      : dx(_dx), dy(_dy), dz(_dz) {}
};

#endif // VELOCITY_H
