#pragma once

#include <GL/glew.h>

#include "stb_image.h"

class Texture {
  public:
    Texture();
    Texture(char *file_loc);

    void LoadTexture();
    void UseTexture();
    void ClearTexture();

    ~Texture();

  private:
    GLuint texture_id;
    int width, height, bit_depth;
    char *file_location;
};

// TEXTURE_H_