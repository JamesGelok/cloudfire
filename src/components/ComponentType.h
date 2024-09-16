#pragma once

#include <stddef.h>

class BaseComponentType {
protected:
  static size_t nextID;
};

template <typename T> struct ComponentType : public BaseComponentType {
  static size_t ID() {
    static size_t id = nextID++;
    return id;
  }
};
