#include "Renderable.h"

Renderable2D::Renderable2D(float _width, float _height, float r, float g,
                           float b)
    : width(_width), height(_height), color(r, g, b), VAO(0), VBO(0) {}

Renderable3D::Renderable3D() : VAO(0), VBO(0), EBO(0) {}
