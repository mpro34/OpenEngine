#pragma once

#include <GL/glew.h>

#include "CommonValues.h"

class Texture {
  public:
    Texture();
    Texture(const char *file_loc);

    bool LoadTexture();
    bool LoadTextureA();

    void UseTexture();
    void ClearTexture();

    ~Texture();

  private:
    GLuint texture_id;
    int width, height, bit_depth;
    const char *file_location;
};

// TEXTURE_H_