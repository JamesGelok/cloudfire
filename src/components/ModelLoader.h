// src/ModelLoader.h
#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "Renderable.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
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

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      glm::vec3 vertex;
      vertex.x = mesh->mVertices[i].x;
      vertex.y = mesh->mVertices[i].y;
      vertex.z = mesh->mVertices[i].z;
      renderable.vertices.push_back(vertex);

      glm::vec3 normal;
      normal.x = mesh->mNormals[i].x;
      normal.y = mesh->mNormals[i].y;
      normal.z = mesh->mNormals[i].z;
      renderable.normals.push_back(normal);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++) {
        renderable.indices.push_back(face.mIndices[j]);
      }
    }

    // Log the results
    std::cout << "Model loaded successfully!" << std::endl;
    std::cout << "Vertices loaded: " << renderable.vertices.size() << std::endl;
    std::cout << "Indices loaded: " << renderable.indices.size() << std::endl;
  }
};

#endif // MODEL_LOADER_H
