#pragma once

#include "../Shader.h"
#include "../WindowConstants.h"
#include "../components/Material.h"
#include "../components/PlayerControlled.h"
#include "../components/Position.h"
#include "../components/Renderable.h"
#include "../components/Rotation.h"
#include "../components/Scale.h"
#include "../core/Entity.h"
#include "../managers/ComponentManager.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>

class RenderSystem {
public:
  RenderSystem(GLFWwindow *window);
  ~RenderSystem();

  void update(float deltaTime, EntityManager &entityManager,
              ComponentManager &componentManager);

  void reset();

private:
  GLFWwindow *window;
  Shader *shader3D;
  glm::mat4 projection;
  glm::vec3 lightDirection;
  glm::vec3 lightColor;
  std::unordered_map<EntityID, GLuint> VAOs3D;
  std::unordered_map<EntityID, GLuint> VBOs3D;

  void initialize();
};
