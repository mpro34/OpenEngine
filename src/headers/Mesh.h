#pragma once

#include <GL/glew.h>

class Mesh {
  public:
    Mesh();

    void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int num_of_vertices, unsigned int num_of_indices);
    void RenderMesh();
    void ClearMesh();

    ~Mesh();

  private:
    GLuint VAO, VBO, IBO;
    GLsizei index_count;
};

// MESH_H_