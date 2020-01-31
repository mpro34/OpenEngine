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
  

}

Skybox::~Skybox() {

}