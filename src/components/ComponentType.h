// src/components/ComponentType.h
#ifndef COMPONENT_TYPE_H
#define COMPONENT_TYPE_H

#include <stddef.h>

class BaseComponentType {
protected:
  static size_t nextID;
};

size_t BaseComponentType::nextID = 0;

template <typename T> struct ComponentType : public BaseComponentType {
  static size_t ID() {
    static size_t id = nextID++;
    return id;
  }
};

#endif // COMPONENT_TYPE_H
