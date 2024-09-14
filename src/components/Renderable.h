#ifndef RENDERABLE_H
#define RENDERABLE_H

struct Renderable {
  float width;
  float height;
  float r, g, b;
  Renderable(float _width = 1.0f, float _height = 1.0f, float _r = 1.0f,
             float _g = 1.0f, float _b = 1.0f)
      : width(_width), height(_height), r(_r), g(_g), b(_b) {}
};

#endif // RENDERABLE_H
