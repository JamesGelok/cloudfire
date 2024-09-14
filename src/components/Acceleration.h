#ifndef ACCELERATION_H
#define ACCELERATION_H

struct Acceleration {
  float ax;
  float ay;
  Acceleration(float _ax = 0.0f, float _ay = 0.0f) : ax(_ax), ay(_ay) {}
};

#endif // ACCELERATION_H
