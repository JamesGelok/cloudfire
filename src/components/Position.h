#ifndef POSITION_H
#define POSITION_H

struct Position {
  float x;
  float y;
  float z; // New field for Z position

  Position(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
      : x(_x), y(_y), z(_z) {}
};

#endif // POSITION_H
