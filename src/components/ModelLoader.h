#pragma once

#include "Renderable.h"
#include <algorithm>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <float.h>
#include <glm/glm.hpp>
#include <iostream>

class ModelLoader {
public:
  static bool loadModel(const std::string &path, Renderable3D &renderable);

private:
  static void processMesh(aiMesh *mesh, Renderable3D &renderable);
};
