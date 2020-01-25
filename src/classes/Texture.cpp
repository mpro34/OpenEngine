#include "../headers/Texture.h"

Texture::Texture() {
  texture_id = 0;
  width = 0;
  height = 0;
  bit_depth = 0;
  file_location = "";
}

Texture::Texture(char *file_loc) {
  texture_id = 0;
  width = 0;
  height = 0;
  bit_depth = 0;
  file_location = file_loc;
}

void Texture::LoadTexture() {
  // Load a texture - unsigned char array is same as byte array (1 char = 1 byte)
  unsigned char *tex_data = stbi_load(file_location, &width, &height, &bit_depth, 0);
  if (!tex_data) {
    printf("Failed to find: %s\n", file_location);
  }

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Unbind and clear the image data
  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(tex_data);
}

void Texture::UseTexture() {
  // The texture unit for use with multiple textures
  glActiveTexture(GL_TEXTURE0); 
  glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::ClearTexture() {
  glDeleteTextures(1, &texture_id);
  texture_id = 0;
  width = 0;
  height = 0;
  bit_depth = 0;
  file_location = "";
}

Texture::~Texture() {
  ClearTexture();
}