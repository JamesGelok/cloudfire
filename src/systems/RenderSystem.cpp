#include "./RenderSystem.h"
#include <iostream>

RenderSystem::RenderSystem(GLFWwindow *win) : window(win) { initialize(); }

RenderSystem::~RenderSystem() {
  delete shader3D;
  for (auto &pair : VAOs3D) {
    glDeleteVertexArrays(1, &pair.second);
  }
  for (auto &pair : VBOs3D) {
    glDeleteBuffers(1, &pair.second);
  }
}

void RenderSystem::reset() {
  delete shader3D;
  for (auto &pair : VAOs3D) {
    glDeleteVertexArrays(1, &pair.second);
  }
  for (auto &pair : VBOs3D) {
    glDeleteBuffers(1, &pair.second);
  }
  VAOs3D.clear();
  VBOs3D.clear();

  initialize();
}

void RenderSystem::initialize() {
  // Initialize shader for 3D rendering
  shader3D = new Shader("shaders/vertex_shader_3D.glsl",
                        "shaders/fragment_shader_3D.glsl");

  // Set up the projection matrix (perspective projection for 3D objects)
  float fov = glm::radians(45.0f);
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  float aspectRatio = (float)width / (float)height;
  float nearPlane = 0.1f;
  float farPlane = 10000.0f;

  projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);

  // Set up directional light properties
  lightDirection = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f));
  lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

  // Enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Set the background color to sky blue
  glClearColor(0.53f, 0.81f, 0.98f, 1.0f);
}

void RenderSystem::update(float deltaTime, EntityManager &entityManager,
                          ComponentManager &componentManager) {
  // Clear buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Find the player entity
  Position *playerPosition = nullptr;
  Rotation *playerRotation = nullptr;

  for (EntityID entity = 0; entity < entityManager.entityCount(); ++entity) {
    const ComponentMask &mask = entityManager.getComponentMask(entity);
    if (mask.test(ComponentType<PlayerControlled>::ID()) &&
        mask.test(ComponentType<Position>::ID()) &&
        mask.test(ComponentType<Rotation>::ID())) {
      playerPosition = componentManager.getComponent<Position>(entity);
      playerRotation = componentManager.getComponent<Rotation>(entity);
      break;
    }
  }

  if (playerPosition == nullptr || playerRotation == nullptr) {
    std::cerr << "Error: No player entity found." << std::endl;
    return;
  }

  // Set up the camera's view matrix
  glm::vec3 cameraOffset(0.0f, 5.0f, 15.0f);

  glm::vec3 forward = playerRotation->quaternion * glm::vec3(0.0f, 0.0f, -1.0f);
  forward.y = 0.0f;
  forward = glm::normalize(forward);

  glm::vec3 cameraPosition =
      glm::vec3(playerPosition->x, playerPosition->y + cameraOffset.y,
                playerPosition->z) -
      forward * cameraOffset.z;

  glm::vec3 cameraTarget =
      glm::vec3(playerPosition->x, playerPosition->y + cameraOffset.y / 2.0f,
                playerPosition->z);

  glm::mat4 view =
      glm::lookAt(cameraPosition, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));

  // Update the projection matrix if the window size changes
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  if (height == 0)
    height = 1; // Prevent division by zero
  float aspectRatio = (float)width / (float)height;
  projection =
      glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

  // Render 3D models
  shader3D->use();
  shader3D->setMat4("projection", glm::value_ptr(projection));
  shader3D->setMat4("view", glm::value_ptr(view));

  shader3D->setVec3("lightDir", lightDirection.x, lightDirection.y,
                    lightDirection.z);
  shader3D->setVec3("lightColor", lightColor.r, lightColor.g, lightColor.b);
  shader3D->setFloat("ambientStrength", 0.5f);

  for (EntityID entity = 0; entity < entityManager.entityCount(); ++entity) {
    const ComponentMask &mask = entityManager.getComponentMask(entity);

    if (mask.test(ComponentType<Renderable3D>::ID()) &&
        mask.test(ComponentType<Position>::ID()) &&
        mask.test(ComponentType<Material>::ID())) {
      auto *position = componentManager.getComponent<Position>(entity);
      auto *renderable = componentManager.getComponent<Renderable3D>(entity);
      auto *material = componentManager.getComponent<Material>(entity);

      // Retrieve rotation component if it exists
      Rotation *rotation = nullptr;
      if (mask.test(ComponentType<Rotation>::ID())) {
        rotation = componentManager.getComponent<Rotation>(entity);
      }

      // Retrieve scale component if it exists
      Scale *scaleComp = nullptr;
      if (mask.test(ComponentType<Scale>::ID())) {
        scaleComp = componentManager.getComponent<Scale>(entity);
      }

      if (position && renderable && material) {
        // Initialize VAO and VBO if not already done
        if (VAOs3D.find(entity) == VAOs3D.end()) {
          GLuint VAO, VBO, EBO;
          glGenVertexArrays(1, &VAO);
          glGenBuffers(1, &VBO);
          glGenBuffers(1, &EBO);

          glBindVertexArray(VAO);
          glBindBuffer(GL_ARRAY_BUFFER, VBO);

          std::vector<float> vertexData;
          for (size_t i = 0; i < renderable->vertices.size(); ++i) {
            vertexData.push_back(renderable->vertices[i].x);
            vertexData.push_back(renderable->vertices[i].y);
            vertexData.push_back(renderable->vertices[i].z);
            vertexData.push_back(renderable->normals[i].x);
            vertexData.push_back(renderable->normals[i].y);
            vertexData.push_back(renderable->normals[i].z);
          }

          glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float),
                       vertexData.data(), GL_STATIC_DRAW);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                       renderable->indices.size() * sizeof(GLuint),
                       &renderable->indices[0], GL_STATIC_DRAW);

          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                                (void *)0);
          glEnableVertexAttribArray(0);
          glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                                (void *)(3 * sizeof(float)));
          glEnableVertexAttribArray(1);

          glBindVertexArray(0);

          VAOs3D[entity] = VAO;
          VBOs3D[entity] = VBO;
        }

        // Build the model matrix
        glm::mat4 model = glm::mat4(1.0f);

        // Apply scaling
        if (scaleComp) {
          model = glm::scale(model, scaleComp->scale);
        }

        // Apply rotation
        if (rotation) {
          model = model * glm::mat4_cast(rotation->quaternion);
        }

        // Apply translation
        model =
            glm::translate(glm::mat4(1.0f),
                           glm::vec3(position->x, position->y, position->z)) *
            model;

        // Set the model matrix in the shader
        shader3D->setMat4("model", glm::value_ptr(model));

        // Set material properties
        shader3D->setVec3("objectColor", material->diffuseColor.r,
                          material->diffuseColor.g, material->diffuseColor.b);
        shader3D->setFloat("specularStrength", material->specularStrength);
        shader3D->setFloat("shininess", material->shininess);

        // Render the model
        glBindVertexArray(VAOs3D[entity]);
        glDrawElements(GL_TRIANGLES, renderable->indices.size(),
                       GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
      }
    }
  }
}
