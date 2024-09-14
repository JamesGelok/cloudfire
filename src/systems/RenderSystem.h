#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../core/Entity.h"
#include <GLFW/glfw3.h>
#include <iostream> // For debug logging

class RenderSystem {
public:
  void update(

      float deltaTime,

      EntityManager &entityManager,

      ComponentManager &componentManager

  );
};

void RenderSystem::update(float deltaTime, EntityManager &entityManager,
                          ComponentManager &componentManager) {
  glClear(GL_COLOR_BUFFER_BIT);

  for (EntityID entity = 0; entity < entityManager.entityCount(); ++entity) {
    const ComponentMask &mask = entityManager.getComponentMask(entity);
    std::cout << "Entity: " << entity << std::endl;

    // Check if the entity has Position and Renderable components
    if (mask.test(ComponentType<Position>::ID()) &&
        mask.test(ComponentType<Renderable>::ID())) {

      auto *position = componentManager.getComponent<Position>(entity);
      auto *renderable = componentManager.getComponent<Renderable>(entity);

      std::cout << "Position: " << position << std::endl;
      std::cout << "Renderable: " << renderable << std::endl;

      if (position && renderable) {
        // Debug logging to confirm the entity's position and size
        std::cout << "Rendering entity at position: (" << position->x << ", "
                  << position->y << "), size: (" << renderable->width << "x"
                  << renderable->height << ")\n";

        glColor3f(renderable->r, renderable->g, renderable->b);
        glBegin(GL_QUADS);
        glVertex2f(position->x - renderable->width / 2,
                   position->y - renderable->height / 2);
        glVertex2f(position->x + renderable->width / 2,
                   position->y - renderable->height / 2);
        glVertex2f(position->x + renderable->width / 2,
                   position->y + renderable->height / 2);
        glVertex2f(position->x - renderable->width / 2,
                   position->y + renderable->height / 2);
        glEnd();
      }
    }
  }

  glfwSwapBuffers(glfwGetCurrentContext());
}

#endif // RENDER_SYSTEM_H
