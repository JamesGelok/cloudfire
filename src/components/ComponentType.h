#include <stddef.h>
#ifndef COMPONENT_TYPE_H
#define COMPONENT_TYPE_H

template <typename T> struct ComponentType {
  static size_t ID() {
    static size_t id = nextID++;
    return id;
  }

private:
  static size_t nextID;
};

template <typename T> size_t ComponentType<T>::nextID = 0;

#endif // COMPONENT_TYPE_H
