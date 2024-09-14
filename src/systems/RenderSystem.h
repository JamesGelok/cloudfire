#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../Shader.h"
#include "../WindowConstants.h"
#include "../components/Position.h"
#include "../components/Renderable.h"
#include "../core/Entity.h"
#include "../managers/ComponentManager.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <unordered_map>

class RenderSystem {
public:
  RenderSystem();
  ~RenderSystem();

  void update(float deltaTime, EntityManager &entityManager,
              ComponentManager &componentManager);

private:
  Shader *shader;
  glm::mat4 projection;
  glm::mat4 view; // Added view matrix for the camera
  std::unordered_map<EntityID, GLuint> VAOs;
  std::unordered_map<EntityID, GLuint> VBOs; // Store VBOs for each entity
};

RenderSystem::RenderSystem() {
  // Initialize the shader
  shader =
      new Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

  // Set up the projection matrix (perspective projection)
  float fov = glm::radians(45.0f); // Field of view (45 degrees is typical)
  float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
  float nearPlane = 0.1f;    // Near clipping plane
  float farPlane = 10000.0f; // Far clipping plane

  projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);

  // Set up a basic view matrix (camera)
  view =
      glm::lookAt(glm::vec3(0.0f, 0.0f, 500.0f), // Pull the camera further back
                  glm::vec3(0.0f, 0.0f, 0.0f),   // Looking at the origin
                  glm::vec3(0.0f, 1.0f, 0.0f)    // Up vector (positive Y axis)
      );

  // Enable blending (optional)
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

RenderSystem::~RenderSystem() {
  delete shader;
  // Clean up VAOs and VBOs
  for (auto &pair : VAOs) {
    glDeleteVertexArrays(1, &pair.second);
  }
  for (auto &pair : VBOs) {
    glDeleteBuffers(1, &pair.second);
  }
}

void RenderSystem::update(float deltaTime, EntityManager &entityManager,
                          ComponentManager &componentManager) {
  shader->use();
  shader->setMat4("projection", glm::value_ptr(projection));
  shader->setMat4("view", glm::value_ptr(view)); // Ensure view matrix is set

  for (EntityID entity = 0; entity < entityManager.entityCount(); ++entity) {
    const ComponentMask &mask = entityManager.getComponentMask(entity);

    // Check if the entity has Position, Renderable, and Rotation components
    if (mask.test(ComponentType<Position>::ID()) &&
        mask.test(ComponentType<Renderable>::ID()) &&
        mask.test(ComponentType<Rotation>::ID())) {

      auto *position = componentManager.getComponent<Position>(entity);
      auto *renderable = componentManager.getComponent<Renderable>(entity);
      auto *rotation = componentManager.getComponent<Rotation>(entity);

      if (position && renderable && rotation) {
        // Initialize VAO and VBO if not already done
        if (VAOs.find(entity) == VAOs.end()) {
          // Set up vertex data for a rectangle (quad)
          float halfWidth = renderable->width / 2.0f;
          float halfHeight = renderable->height / 2.0f;

          GLfloat vertices[] = {
              -halfWidth,          -halfHeight,         renderable->color.r,
              renderable->color.g, renderable->color.b, halfWidth,
              -halfHeight,         renderable->color.r, renderable->color.g,
              renderable->color.b, halfWidth,           halfHeight,
              renderable->color.r, renderable->color.g, renderable->color.b,
              -halfWidth,          halfHeight,          renderable->color.r,
              renderable->color.g, renderable->color.b,
          };

          GLuint indices[] = {0, 1, 2, 2, 3, 0};

          GLuint VAO, VBO, EBO;
          glGenVertexArrays(1, &VAO);
          glGenBuffers(1, &VBO);
          glGenBuffers(1, &EBO);

          glBindVertexArray(VAO);
          glBindBuffer(GL_ARRAY_BUFFER, VBO);
          glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                       GL_STATIC_DRAW);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                       GL_STATIC_DRAW);

          glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                (void *)0);
          glEnableVertexAttribArray(0);
          glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                (void *)(2 * sizeof(float)));
          glEnableVertexAttribArray(1);
          glBindVertexArray(0);

          VAOs[entity] = VAO;
          VBOs[entity] = VBO;
        }

        // Create model matrix for this entity
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(
            model, glm::vec3(position->x, position->y, position->z));

        // Apply rotation from quaternion
        model = model * glm::mat4_cast(rotation->quaternion);

        // Set model matrix in the shader
        shader->setMat4("model", glm::value_ptr(model));

        // Bind VAO and draw the entity
        glBindVertexArray(VAOs[entity]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
      }
    }
  }
}

#endif // RENDER_SYSTEM_H
