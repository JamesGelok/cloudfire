#ifndef POSITION_H
#define POSITION_H

struct Position {
  float x;
  float y;
  Position(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}
};

#endif // POSITION_H
