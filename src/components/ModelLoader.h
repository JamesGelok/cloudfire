// src/components/ModelLoader.h
#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "Renderable.h"
#include <algorithm> // For std::max
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <float.h> // For FLT_MAX
#include <glm/glm.hpp>
#include <iostream>

class ModelLoader {
public:
  // Load a 3D model and store the data in a Renderable3D component
  static bool loadModel(const std::string &path, Renderable3D &renderable) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
      std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
      return false;
    }

    // Process the first mesh in the scene (for simplicity)
    if (scene->mNumMeshes > 0) {
      aiMesh *mesh = scene->mMeshes[0];
      processMesh(mesh, renderable);
    }

    return true;
  }

private:
  static void processMesh(aiMesh *mesh, Renderable3D &renderable) {
    // Clear any existing data in renderable
    renderable.vertices.clear();
    renderable.normals.clear();
    renderable.indices.clear();

    // Variables to find the center of the model
    glm::vec3 minVertex(FLT_MAX);
    glm::vec3 maxVertex(-FLT_MAX);

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      glm::vec3 vertex;
      vertex.x = mesh->mVertices[i].x;
      vertex.y = mesh->mVertices[i].y;
      vertex.z = mesh->mVertices[i].z;
      renderable.vertices.push_back(vertex);

      // Update min and max vertices
      minVertex = glm::min(minVertex, vertex);
      maxVertex = glm::max(maxVertex, vertex);

      glm::vec3 normal;
      normal.x = mesh->mNormals[i].x;
      normal.y = mesh->mNormals[i].y;
      normal.z = mesh->mNormals[i].z;
      renderable.normals.push_back(normal);
    }

    // Calculate the center and size of the model
    glm::vec3 center = (minVertex + maxVertex) * 0.5f;
    glm::vec3 size = maxVertex - minVertex;

    // Compute the maximum component of size
    float maxComponent = std::max(size.x, std::max(size.y, size.z));
    if (maxComponent == 0.0f) {
      maxComponent = 1.0f; // Prevent division by zero
    }
    float scaleFactor = 1.0f / maxComponent; // Normalize to unit size

    // Adjust vertices to center and normalize the model
    for (auto &vertex : renderable.vertices) {
      vertex = (vertex - center) * scaleFactor;
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++) {
        renderable.indices.push_back(face.mIndices[j]);
      }
    }

    // Log the results
    std::cout << "Model loaded and normalized successfully!" << std::endl;
    std::cout << "Vertices loaded: " << renderable.vertices.size() << std::endl;
    std::cout << "Indices loaded: " << renderable.indices.size() << std::endl;
  }
};

#endif // MODEL_LOADER_H
