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

// Function to check for OpenGL errors
void checkGLError(const std::string &message) {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    std::cerr << "[OpenGL ERROR] " << message << " - Error Code: " << err
              << std::endl;
  }
}

class RenderSystem {
public:
  RenderSystem();
  ~RenderSystem();

  void update(float deltaTime, EntityManager &entityManager,
              ComponentManager &componentManager);

private:
  Shader *shader2D;
  Shader *shader3D;
  glm::mat4 projection;
  glm::mat4 view; // View matrix for the camera
  std::unordered_map<EntityID, GLuint> VAOs2D;
  std::unordered_map<EntityID, GLuint> VBOs2D;
  std::unordered_map<EntityID, GLuint> VAOs3D;
  std::unordered_map<EntityID, GLuint> VBOs3D;
};

RenderSystem::RenderSystem() {
  // Initialize shaders for 2D and 3D rendering
  std::cout << "[RenderSystem] Initializing shaders..." << std::endl;
  shader2D = new Shader("shaders/vertex_shader_2D.glsl",
                        "shaders/fragment_shader_2D.glsl");
  shader3D = new Shader("shaders/vertex_shader_3D.glsl",
                        "shaders/fragment_shader_3D.glsl");

  checkGLError("Shader initialization");

  // Set up the projection matrix (perspective projection for 3D objects)
  std::cout << "[RenderSystem] Setting up projection matrix..." << std::endl;
  float fov = glm::radians(45.0f);
  float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
  float nearPlane = 0.1f;
  float farPlane = 10000.0f;

  projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);

  checkGLError("Projection matrix setup");

  // Set up a basic view matrix (camera)
  std::cout << "[RenderSystem] Setting up view matrix..." << std::endl;
  view = glm::lookAt(glm::vec3(0.0f, 0.0f, 500.0f), // Camera position
                     glm::vec3(0.0f, 0.0f, 0.0f),   // Look at the origin
                     glm::vec3(0.0f, 1.0f, 0.0f) // Up vector (positive Y axis)
  );

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  std::cout << "[RenderSystem] Blending enabled." << std::endl;

  checkGLError("Blending setup");
}

RenderSystem::~RenderSystem() {
  std::cout << "[RenderSystem] Cleaning up shaders and buffers..." << std::endl;
  delete shader2D;
  delete shader3D;
  for (auto &pair : VAOs2D) {
    glDeleteVertexArrays(1, &pair.second);
  }
  for (auto &pair : VBOs2D) {
    glDeleteBuffers(1, &pair.second);
  }
  for (auto &pair : VAOs3D) {
    glDeleteVertexArrays(1, &pair.second);
  }
  for (auto &pair : VBOs3D) {
    glDeleteBuffers(1, &pair.second);
  }
  std::cout << "[RenderSystem] Cleanup complete." << std::endl;

  checkGLError("Cleanup");
}

void RenderSystem::update(float deltaTime, EntityManager &entityManager,
                          ComponentManager &componentManager) {
  std::cout << "[RenderSystem] Starting update..." << std::endl;

  // Render 2D objects
  std::cout << "[RenderSystem] Rendering 2D objects..." << std::endl;
  shader2D->use();
  shader2D->setMat4("projection", glm::value_ptr(projection));
  shader2D->setMat4("view", glm::value_ptr(view));

  checkGLError("2D Shader use");

  for (EntityID entity = 0; entity < entityManager.entityCount(); ++entity) {
    const ComponentMask &mask = entityManager.getComponentMask(entity);

    if (mask.test(ComponentType<Position>::ID()) &&
        mask.test(ComponentType<Renderable2D>::ID()) &&
        mask.test(ComponentType<Rotation>::ID())) {

      auto *position = componentManager.getComponent<Position>(entity);
      auto *renderable = componentManager.getComponent<Renderable2D>(entity);
      auto *rotation = componentManager.getComponent<Rotation>(entity);

      if (position && renderable && rotation) {
        std::cout << "[RenderSystem] Found 2D object, entity ID: " << entity
                  << std::endl;
        if (VAOs2D.find(entity) == VAOs2D.end()) {
          std::cout << "[RenderSystem] Initializing VAO for 2D entity: "
                    << entity << std::endl;
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

          VAOs2D[entity] = VAO;
          VBOs2D[entity] = VBO;
        }

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(
            model, glm::vec3(position->x, position->y, position->z));
        model = model * glm::mat4_cast(rotation->quaternion);

        shader2D->setMat4("model", glm::value_ptr(model));

        glBindVertexArray(VAOs2D[entity]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        checkGLError("2D object render");
      }
    }
  }

  // Render 3D models
  std::cout << "[RenderSystem] Rendering 3D objects..." << std::endl;
  shader3D->use();
  shader3D->setMat4("projection", glm::value_ptr(projection));
  shader3D->setMat4("view", glm::value_ptr(view));

  checkGLError("3D Shader use");

  for (EntityID entity = 0; entity < entityManager.entityCount(); ++entity) {
    const ComponentMask &mask = entityManager.getComponentMask(entity);

    if (mask.test(ComponentType<Renderable3D>::ID()) &&
        mask.test(ComponentType<Position>::ID())) {

      auto *position = componentManager.getComponent<Position>(entity);
      auto *renderable = componentManager.getComponent<Renderable3D>(entity);

      if (position && renderable) {
        shader3D->setColor("objectColor", 0.0f, 0.0f, 1.0f); // Blue color
        std::cout << "[RenderSystem] Found 3D object, entity ID: " << entity
                  << std::endl;
        if (VAOs3D.find(entity) == VAOs3D.end()) {
          std::cout << "[RenderSystem] Initializing VAO for 3D entity: "
                    << entity << std::endl;
          GLuint VAO, VBO, EBO;
          glGenVertexArrays(1, &VAO);
          glGenBuffers(1, &VBO);
          glGenBuffers(1, &EBO);

          glBindVertexArray(VAO);
          glBindBuffer(GL_ARRAY_BUFFER, VBO);
          glBufferData(GL_ARRAY_BUFFER,
                       renderable->vertices.size() * sizeof(glm::vec3),
                       &renderable->vertices[0], GL_STATIC_DRAW);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                       renderable->indices.size() * sizeof(GLuint),
                       &renderable->indices[0], GL_STATIC_DRAW);

          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                                (void *)0);
          glEnableVertexAttribArray(0);

          glBindVertexArray(0);

          VAOs3D[entity] = VAO;
          VBOs3D[entity] = VBO;
        }

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(
            model, glm::vec3(position->x, position->y, position->z));

        shader3D->setMat4("model", glm::value_ptr(model));

        glBindVertexArray(VAOs3D[entity]);
        glDrawElements(GL_TRIANGLES, renderable->indices.size(),
                       GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        checkGLError("3D object render");
      }
    }
  }
  std::cout << "[RenderSystem] Update complete." << std::endl;
}

#endif // RENDER_SYSTEM_H
