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
  std::unordered_map<EntityID, GLuint> VAOs;
  std::unordered_map<EntityID, GLuint> VBOs; // Store VBOs for each entity
};

RenderSystem::RenderSystem() {
  // Initialize the shader
  shader =
      new Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

  // Set up the projection matrix (orthographic projection)
  projection = glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 0.0f,
                          static_cast<float>(WINDOW_HEIGHT));

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
  // glClear(GL_COLOR_BUFFER_BIT);

  shader->use();
  shader->setMat4("projection", glm::value_ptr(projection));

  for (EntityID entity = 0; entity < entityManager.entityCount(); ++entity) {
    const ComponentMask &mask = entityManager.getComponentMask(entity);

    // Check if the entity has Position and Renderable components
    if (mask.test(ComponentType<Position>::ID()) &&
        mask.test(ComponentType<Renderable>::ID())) {

      auto *position = componentManager.getComponent<Position>(entity);
      auto *renderable = componentManager.getComponent<Renderable>(entity);

      if (position && renderable) {
        // Initialize VAO and VBO if not already done
        if (VAOs.find(entity) == VAOs.end()) {
          // Set up vertex data for a rectangle (quad)
          float halfWidth = renderable->width / 2.0f;
          float halfHeight = renderable->height / 2.0f;

          // Use renderable->color (which is a glm::vec3) for the color
          GLfloat vertices[] = {
              // Positions           // Colors
              -halfWidth,          -halfHeight,         renderable->color.r,
              renderable->color.g, renderable->color.b, halfWidth,
              -halfHeight,         renderable->color.r, renderable->color.g,
              renderable->color.b, halfWidth,           halfHeight,
              renderable->color.r, renderable->color.g, renderable->color.b,
              -halfWidth,          halfHeight,          renderable->color.r,
              renderable->color.g, renderable->color.b};

          GLuint indices[] = {
              0, 1, 2, // First triangle
              2, 3, 0  // Second triangle
          };

          // Generate and bind VAO
          GLuint VAO, VBO, EBO;
          glGenVertexArrays(1, &VAO);
          glGenBuffers(1, &VBO);
          glGenBuffers(1, &EBO);

          glBindVertexArray(VAO);

          // Bind and set VBO
          glBindBuffer(GL_ARRAY_BUFFER, VBO);
          glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                       GL_STATIC_DRAW);

          // Bind and set EBO (Element Buffer Object for indices)
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                       GL_STATIC_DRAW);

          // Set vertex attributes (position and color)
          glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                (void *)0); // Position
          glEnableVertexAttribArray(0);

          glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                (void *)(2 * sizeof(float))); // Color
          glEnableVertexAttribArray(1);

          // Unbind VAO
          glBindVertexArray(0);

          // Store the VAO and VBO in the map
          VAOs[entity] = VAO;
          VBOs[entity] = VBO;
        }

        // Create model matrix for this entity
        glm::mat4 model = glm::mat4(1.0f);
        model =
            glm::translate(model, glm::vec3(position->x, position->y, 0.0f));

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
