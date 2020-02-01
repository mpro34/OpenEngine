#include "../headers/Skybox.h"

Skybox::Skybox() {

}

Skybox::Skybox(std::vector<std::string> face_locations) {
  // Shader setup
  sky_shader = new Shader();
  sky_shader->CreateFromFiles("src/Shaders/skybox.vert", "src/Shaders/skybox.frag");

  uniform_projection = sky_shader->GetProjectionLocation();
  uniform_view = sky_shader->GetViewLocation();

  // Texture setup
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

  int width, height, bit_depth;

  for (size_t i = 0; i < 6; i++) {
    unsigned char *tex_data = stbi_load(face_locations[i].c_str(), &width, &height, &bit_depth, 0);
    if (!tex_data) {
      printf("Failed to find: %s\n", face_locations[i].c_str());
      return;
    }

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
    stbi_image_free(tex_data);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Mesh Setup
  unsigned int skybox_indices[] = {
    //front
    0, 1, 2,
    2, 1, 3,
    // right
    2, 3, 5,
    5, 3, 7,
    // back
    5, 7, 4,
    4, 7, 6,
    // left
    4, 6, 0,
    0, 6, 1,
    // top
    4, 0, 5,
    5, 0, 2,
    // bottom
    1, 6, 3,
    3, 6, 7
  };

  float skybox_vertices[] = {
    -1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

    -1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
  };

  sky_mesh = new Mesh();
  sky_mesh->CreateMesh(skybox_vertices, skybox_indices, 64, 36);
}

void Skybox::DrawSkybox(glm::mat4 view_matrix, glm::mat4 projection_matrix) {

  view_matrix = glm::mat4(glm::mat3(view_matrix)); // clear out all data that would position the camera outside of skybox
  
  // Turn off depth mask only during drawing of skybox.
  glDepthMask(GL_FALSE);

  sky_shader->UseShader();

  glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));
  glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(view_matrix));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

  sky_shader->Validate();

  sky_mesh->RenderMesh();

  glDepthMask(GL_TRUE);
}

Skybox::~Skybox() {

}