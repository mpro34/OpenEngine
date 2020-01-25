#include "../headers/Mesh.h"

Mesh::Mesh() {
  VAO = 0;
  VBO = 0;
  IBO = 0;
  index_count = 0;
}

void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int num_of_vertices, unsigned int num_of_indices) {
  index_count = num_of_indices;

  glGenVertexArrays(1, &VAO); 
  glBindVertexArray(VAO);

  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * num_of_indices, indices, GL_STATIC_DRAW);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * num_of_vertices, vertices, GL_STATIC_DRAW);

  // Map the vertices in the vertices array for the vertex shader, first 3 are primitive position, last 2 are texture coords.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
  glEnableVertexAttribArray(2);


  // Unbind buffers
  glBindBuffer(GL_ARRAY_BUFFER, 0);         // unbind the VBO
  glBindVertexArray(0);                     // unbind the VAO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind the IBO (must happen after unbinding the VAO!)
}

void Mesh::RenderMesh() {
  // Render the Mesh created above
  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);     
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::ClearMesh() {
  if (IBO != 0) {
    glDeleteBuffers(1, &IBO);
    IBO = 0;
  }

  if (VBO != 0) {
    glDeleteBuffers(1, &VBO);
    VBO = 0;
  }

  if (VAO != 0) {
    glDeleteVertexArrays(1, &VAO);
    VAO = 0;
  }

  index_count = 0;
}

Mesh::~Mesh() {
  ClearMesh();
}