#include "ModelLoader.h"

bool ModelLoader::loadModel(const std::string &path, Renderable3D &renderable) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
    return false;
  }

  if (scene->mNumMeshes > 0) {
    aiMesh *mesh = scene->mMeshes[0];
    processMesh(mesh, renderable);
  }

  return true;
}

void ModelLoader::processMesh(aiMesh *mesh, Renderable3D &renderable) {
  renderable.vertices.clear();
  renderable.normals.clear();
  renderable.indices.clear();

  glm::vec3 minVertex(FLT_MAX);
  glm::vec3 maxVertex(-FLT_MAX);

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    glm::vec3 vertex;
    vertex.x = mesh->mVertices[i].x;
    vertex.y = mesh->mVertices[i].y;
    vertex.z = mesh->mVertices[i].z;
    renderable.vertices.push_back(vertex);

    minVertex = glm::min(minVertex, vertex);
    maxVertex = glm::max(maxVertex, vertex);

    glm::vec3 normal;
    normal.x = mesh->mNormals[i].x;
    normal.y = mesh->mNormals[i].y;
    normal.z = mesh->mNormals[i].z;
    renderable.normals.push_back(normal);
  }

  glm::vec3 center = (minVertex + maxVertex) * 0.5f;
  glm::vec3 size = maxVertex - minVertex;

  float maxComponent = std::max(size.x, std::max(size.y, size.z));
  if (maxComponent == 0.0f) {
    maxComponent = 1.0f;
  }
  float scaleFactor = 1.0f / maxComponent;

  for (auto &vertex : renderable.vertices) {
    vertex = (vertex - center) * scaleFactor;
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      renderable.indices.push_back(face.mIndices[j]);
    }
  }

  std::cout << "Model loaded and normalized successfully!" << std::endl;
  std::cout << "Vertices loaded: " << renderable.vertices.size() << std::endl;
  std::cout << "Indices loaded: " << renderable.indices.size() << std::endl;
}
