#ifndef ACCELERATION_H
#define ACCELERATION_H

struct Acceleration {
  float ax;
  float ay;
  float az;

  Acceleration(float _ax = 0.0f, float _ay = 0.0f, float _az = 0.0f)
      : ax(_ax), ay(_ay), az(_az) {}
};

#endif
