#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model {
  public:
    Model();

    void LoadModel(const std::string &file_name);
    void RenderModel();
    void ClearModel();

    ~Model();

  private:
    void LoadNode(aiNode *node, const aiScene *scene);
    void LoadMesh(aiMesh *mesh, const aiScene *scene);
    void LoadMaterials(const aiScene *scene);

    std::vector<Mesh*> mesh_list;
    std::vector<Texture*> texture_list;
    std::vector<unsigned int> mesh_to_texture;
};

// MODEL_H_